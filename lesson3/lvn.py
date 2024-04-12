from dataclasses import dataclass
import sys
from typing import Dict, List, Optional, Union

import bril_utils.bb
from bril_utils.fixed_point import run_to_converge


class LVN:
    def run(self, func) -> bool:
        insts = func["instrs"]

        ret = False
        for block in bril_utils.bb.iter_blocks(insts):
            block_changed = self._run_on_block(block)
            ret = ret or block_changed

        return ret

    @run_to_converge
    def _run_on_block(self, block: bril_utils.bb.Block) -> bool:
        value_table = _ValueTable()

        updated = False
        for inst in block.insts:
            if "args" in inst:
                # Replace all arguments with the canonical variables corresponding to their values.
                arg_numberings = []
                canonical_args = []
                for arg in inst["args"]:
                    numbering = value_table.get_var_value(arg)
                    if numbering is None:
                        # This argument is not numbered. We cannot optimize this instruction.
                        break
                    arg_numberings.append(numbering)
                    canonical_args.append(value_table.get_canonical_var(numbering))
                else:
                    assert None not in canonical_args
                    inst["args"] = canonical_args

            if "dest" not in inst:
                # The instruction is a side-effect.
                continue

            op = inst["op"]
            if op not in _VALUE_OPS:
                # The instruction does not compute a value that could benefit value numbering.
                continue

            if op == "const":
                value = _AbstractValue.create_constant(op, inst["value"])
            else:
                if len(arg_numberings) < len(inst["args"]):
                    # Not all arguments are numbered. We cannot optimize this instruction.
                    continue
                value = _AbstractValue.create(op, arg_numberings)

            dest = inst["dest"]
            canonical_var = value_table.insert_var_def(dest, value)
            if canonical_var != dest:
                # The value of the variable defined by this instruction has already been computed.
                # Replace this instruction with an `id` instruction that copies the value of the
                # canonical variable which holds the already computed value.
                inst["op"] = "id"
                inst["args"] = [canonical_var]
                updated = True

        return updated


@dataclass
class _AbstractValue:
    @staticmethod
    def create_constant(op: str, value: Union[int, bool]) -> "_AbstractValue":
        return _AbstractValue(op, value, [])

    @staticmethod
    def create(op: str, args: List[int]) -> "_AbstractValue":
        return _AbstractValue(op, None, sorted(args))

    op: str
    constant: Optional[Union[int, bool]]
    args: List[int]


@dataclass
class _ValueTableEntry:
    value: _AbstractValue
    vars: List[str]


class _ValueTable:
    def __init__(self) -> None:
        self._rows: List[_ValueTableEntry] = []
        self._var_values: Dict[str, int] = dict()

    def insert_var_def(self, var: str, value: _AbstractValue) -> str:
        # If the variable is already present in the table, we need to remove it from the table.
        if var in self._var_values:
            value_number = self._var_values[var]
            row = self._rows[value_number]
            row.vars.remove(var)
            self._var_values.pop(var)

        # Check if the abstract value is already in the table.
        for value_number, row in enumerate(self._rows):
            if row.value == value:
                # The abstract value is already present. Its value number is `value_number`.
                row.vars.append(var)
                self._var_values[var] = value_number
                return row.vars[0]

        # The abstract value is not in the table.
        value_number = len(self._rows)
        self._rows.append(_ValueTableEntry(value, [var]))
        self._var_values[var] = value_number
        return var

    def get_var_value(self, var: str) -> Optional[int]:
        return self._var_values.get(var)

    def get_canonical_var(self, numbering: int) -> Optional[str]:
        row = self._rows[numbering]
        if len(row.vars) == 0:
            return None
        return row.vars[0]


_VALUE_OPS = set(
    [
        "const",
        "add",
        "sub",
        "mul",
        "div",
        "eq",
        "lt",
        "gt",
        "le",
        "ge",
        "not",
        "and",
        "or",
        "id",
    ]
)

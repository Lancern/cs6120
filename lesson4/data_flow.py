from abc import abstractmethod, ABC
from typing import Any, Dict, List

from bril_utils.bb import Block, collect_blocks
from bril_utils.cfg import CFG


class Lattice[T](ABC):
    @abstractmethod
    def create(self) -> T: ...

    @abstractmethod
    def clone(self, value: T) -> T: ...

    @abstractmethod
    def merge(self, a: T, b: T) -> T: ...

    def merge_many(self, *values: List[T]) -> T:
        if len(values) == 0:
            return self.create()
        if len(values) == 1:
            return values[0]

        result = self.merge(values[0], values[1])
        for i in values[2:]:
            result = self.merge(result, i)
        return result

    @abstractmethod
    def update(self, before: T, instr) -> T: ...

    @abstractmethod
    def is_subset(self, subset: T, superset: T) -> bool: ...


class DataFlowSolver[T]:
    """
    A generic data flow solver.

    The data flow solver acts on an abstract structure called "lattice", represented by
    objects of the Lattice class.
    """

    def __init__(self, lattice: Lattice[T], func) -> None:
        self._lattice = lattice
        self._func = func

        self._state_before_instr: Dict[Any, T] = {}
        self._state_after_instr: Dict[Any, T] = {}
        self._state_before_bb: Dict[Block, T] = {}
        self._state_after_bb: Dict[Block, T] = {}

        self._run_data_flow()

    @property
    def func(self):
        return self._func

    def get_state_before_instr(self, instr) -> T:
        return self._state_before_instr[instr]

    def get_state_after_instr(self, instr) -> T:
        return self._state_after_instr[instr]

    def get_state_before_bb(self, bb: Block) -> T:
        return self._state_before_bb[bb]

    def get_state_after_bb(self, bb: Block) -> T:
        return self._state_after_bb[bb]

    def _run_data_flow(self):
        bb_list = collect_blocks(self._func["instrs"])
        cfg = CFG(bb_list)

        converged = False
        while not converged:
            converged = True
            for bb in bb_list:
                updated = self._run_data_flow_on_bb(cfg, bb)
                converged = converged and not updated

    def _run_data_flow_on_bb(self, cfg: CFG, bb: Block) -> bool:
        pred = cfg.get_predecessors(bb)
        pred_exit_states = [self._state_after_bb[b] for b in pred]

        def update_state_entry(state_dict, state_node, state) -> bool:
            if state_node not in state_dict or not self._lattice.is_subset(
                state, state_dict[state_node]
            ):
                state_dict[state_node] = state
                return True
            return False

        curr_state = self._lattice.merge_many(*pred_exit_states)

        if not update_state_entry(self._state_before_bb, bb, curr_state):
            return False

        self._state_before_bb[bb] = curr_state
        for instr in bb.insts:
            if not update_state_entry(self._state_before_instr, instr, curr_state):
                return True

            curr_state = self._lattice.update(curr_state, instr)

            if not update_state_entry(self._state_after_instr, instr, curr_state):
                return True

        update_state_entry(self._state_after_bb, bb, curr_state)
        return True

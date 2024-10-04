from typing import override, Set

from .data_flow import DataFlowDirection, DataFlowSolver, Lattice


class LiveVarLattice(Lattice[Set[str]]):
    """
    Lattice implementation for live variable analysis.
    """

    @override
    def create(self) -> Set[str]:
        return set()

    @override
    def clone(self, value: Set[str]) -> Set[str]:
        return value.copy()

    @override
    def merge(self, a: Set[str], b: Set[str]) -> Set[str]:
        return a | b

    @override
    def update(self, before: Set[str], instr) -> Set[str]:
        ret = before.copy()
        if "op" in instr:
            if "dest" in instr and instr["dest"] in ret:
                ret.remove(instr["dest"])
            if "args" in instr:
                ret.update(instr["args"])
        return ret

    @override
    def is_subset(self, subset: Set[str], superset: Set[str]) -> bool:
        return subset <= superset


def create_live_var_analysis(func) -> DataFlowSolver:
    return DataFlowSolver(LiveVarLattice(), func, DataFlowDirection.BACKWARD)

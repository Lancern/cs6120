from abc import abstractmethod, ABC
from enum import auto, Enum
from typing import Any, Dict, List

from bril_utils.bb import Block, collect_blocks
from bril_utils.cfg import CFG
from bril_utils.collections.object_map import ObjectMap


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


class DataFlowDirection(Enum):
    FORWARD = auto()
    BACKWARD = auto()


class DataFlowSolver[T]:
    """
    A generic data flow solver.

    The data flow solver acts on an abstract structure called "lattice", represented by
    objects of the Lattice class.
    """

    def __init__(
        self,
        lattice: Lattice[T],
        func,
        dir: DataFlowDirection = DataFlowDirection.FORWARD,
    ) -> None:
        self._lattice = lattice
        self._func = func
        self._dir = dir

        self._state_before_instr: ObjectMap[Any, T] = ObjectMap()
        self._state_after_instr: ObjectMap[Any, T] = ObjectMap()
        self._state_before_bb: ObjectMap[Block, T] = ObjectMap()
        self._state_after_bb: ObjectMap[Block, T] = ObjectMap()

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

        if self._dir == DataFlowDirection.BACKWARD:
            bb_list.reverse()

        converged = False
        while not converged:
            converged = True
            for bb in bb_list:
                updated = self._run_data_flow_on_bb(cfg, bb)
                converged = converged and not updated

    def _run_data_flow_on_bb(self, cfg: CFG, bb: Block) -> bool:
        if self._dir == DataFlowDirection.FORWARD:
            get_predecessors = cfg.get_predecessors
            state_before_bb = self._state_before_bb
            state_after_bb = self._state_after_bb
            state_before_instr = self._state_before_instr
            state_after_instr = self._state_after_instr
            bb_instrs = bb.insts
        else:
            get_predecessors = cfg.get_successors
            state_before_bb = self._state_after_bb
            state_after_bb = self._state_before_bb
            state_before_instr = self._state_after_instr
            state_after_instr = self._state_before_instr
            bb_instrs = list(reversed(bb.insts))

        pred = get_predecessors(bb)
        pred_exit_states = [state_after_bb[b] for b in pred]

        curr_state = self._lattice.merge_many(*pred_exit_states)

        if not self._update_state_entry(state_before_bb, bb, curr_state):
            return False

        state_before_bb[bb] = curr_state
        for instr in bb_instrs:
            if not self._update_state_entry(
                state_before_instr, instr, curr_state
            ):
                return True

            curr_state = self._lattice.update(curr_state, instr)

            if not self._update_state_entry(state_after_instr, instr, curr_state):
                return True

        self._update_state_entry(state_after_bb, bb, curr_state)
        return True

    def _update_state_entry(self, state_dict, state_node, state) -> bool:
        if state_node not in state_dict or not self._lattice.is_subset(
            state, state_dict[state_node]
        ):
            state_dict[state_node] = state
            return True
        return False

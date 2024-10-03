from typing import Dict, List, TYPE_CHECKING

if TYPE_CHECKING:
    from .bb import Block


class CFG:
    def __init__(self, bb_list: List["Block"]) -> None:
        assert len(bb_list) > 0
        self._bb_list = list(bb_list)

        self._bb_name: Dict[str, "Block"] = {}
        self._bb_succ: Dict["Block", List["Block"]] = {}
        self._bb_pred: Dict["Block", List["Block"]] = {}

        def add_cfg_edge(from_bb: "Block", to_bb: "Block"):
            self._bb_succ[from_bb].append(to_bb)
            self._bb_pred[to_bb].append(from_bb)

        # Allocate entries in the dictionaries for each basic block.
        for bb in bb_list:
            if bb.name is not None:
                self._bb_name[bb.name] = bb
            self._bb_succ[bb] = []
            self._bb_pred[bb] = []

        for idx, bb in enumerate(bb_list):
            term = bb.insts[-1]
            if "op" not in term or "labels" not in term:
                if idx + 1 < len(bb_list):
                    add_cfg_edge(bb, bb_list[idx + 1])
                continue

            opcode = term["op"]
            labels = term["labels"]
            if opcode == "jmp":
                assert len(labels) == 1
                add_cfg_edge(bb, self._bb_name[labels[0]])
            elif opcode == "br":
                assert len(labels) == 2
                add_cfg_edge(bb, self._bb_name[labels[0]])
                add_cfg_edge(bb, self._bb_name[labels[1]])
            else:
                raise NotImplementedError("unknown opcode")

    @property
    def entry(self) -> "Block":
        return self._bb_list[0]

    @property
    def exits(self) -> List["Block"]:
        def is_exit(bb: "Block") -> bool:
            return len(self._bb_succ[bb]) == 0
        return list(filter(is_exit, self._bb_list))

    def get_block_by_name(self, name: str) -> "Block":
        return self._bb_name[name]

    def get_successors(self, bb: "Block") -> List["Block"]:
        return list(self._bb_succ[bb])

    def get_predecessors(self, bb: "Block") -> List["Block"]:
        return list(self._bb_pred[bb])

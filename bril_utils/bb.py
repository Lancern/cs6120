from dataclasses import dataclass
from typing import Optional


@dataclass
class Block:
    name: Optional[str]
    insts: list
    first_inst_idx: int


def iter_blocks(insts):
    TERMINATOR_OPS = ["jmp", "br", "ret"]

    curr_block = Block(None, [], 0)
    for (idx, i) in enumerate(insts):
        if "label" in i:
            if len(curr_block.insts) > 0:
                yield curr_block
                curr_block = Block(i["label"], [], idx + 1)
            else:
                curr_block.name = i["label"]
                curr_block.first_inst_idx = idx + 1
        else:
            curr_block.insts.append(i)
            if i["op"] in TERMINATOR_OPS:
                yield curr_block
                curr_block = Block(None, [], idx + 1)

    if len(curr_block.insts) > 0:
        yield curr_block


def collect_blocks(insts):
    return list(iter_blocks(insts))

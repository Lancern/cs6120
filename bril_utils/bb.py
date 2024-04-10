from dataclasses import dataclass
from typing import Optional


@dataclass
class Block:
    name: Optional[str]
    insts: list


def iter_blocks(insts):
    TERMINATOR_OPS = ["jmp", "br", "ret"]

    curr_block = Block(None, [])
    for i in insts:
        if "label" in i:
            if len(curr_block.insts) > 0:
                yield curr_block
                curr_block = Block(i["label"], [])
            else:
                curr_block.name = i["label"]
        else:
            curr_block.insts.append(i)
            if i["op"] in TERMINATOR_OPS:
                yield curr_block
                curr_block = Block(None, [])

    if len(curr_block.insts) > 0:
        yield curr_block


def collect_blocks(insts):
    return list(iter_blocks(insts))

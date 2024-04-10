import json
import sys

import bril_utils.bb

prog = json.load(sys.stdin)
funcs = prog["functions"]

num_funcs = len(funcs)
print(f"{num_funcs} function(s)")

for f in funcs:
    func_name = f["name"]
    blocks = bril_utils.bb.collect_blocks(f["instrs"])
    num_blocks = len(blocks)
    print(f"Function \"{func_name}\": {num_blocks} basic block(s)")

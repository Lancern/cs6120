import json
import sys

from .local_dce import LocalDCE


prog = json.load(sys.stdin)

if sys.argv[1] == "dce":
    optimizer = LocalDCE()
else:
    print(f"Unknown optimizer \"{sys.argv[1]}\"", file=sys.stderr)
    exit(1)

for f in prog["functions"]:
    func_name = f["name"]
    if optimizer.run(f):
        print(f"Function {func_name}: optimized.", file=sys.stderr)
    else:
        print(f"Function {func_name}: not optimized.", file=sys.stderr)

json.dump(prog, sys.stdout)

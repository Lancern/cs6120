import json
import sys

from bril_utils.fixed_point import run_to_converge

from .local_dce import LocalDCE
from .lvn import LVN


class CombinedOptimizer:
    def __init__(self) -> None:
        self._dce = LocalDCE()
        self._lvn = LVN()

    @run_to_converge
    def run(self, func) -> bool:
        dce_updated = self._dce.run(func)
        lvn_updated = self._lvn.run(func)
        return dce_updated or lvn_updated


prog = json.load(sys.stdin)

if sys.argv[1] == "dce":
    optimizer = LocalDCE()
elif sys.argv[1] == "lvn":
    optimizer = LVN()
elif sys.argv[1] == "lvn+dce":
    optimizer = CombinedOptimizer()
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

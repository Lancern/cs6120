import json
import sys

from .live_var import create_live_var_analysis


prog = json.load(sys.stdin)

if sys.argv[1] == "live_var":
    analysis_factory = create_live_var_analysis
else:
    print(f"Unknown analysis \"{sys.argv[1]}\"", file=sys.stderr)
    exit(1)

for f in prog["functions"]:
    func_name = f["name"]
    analysis = analysis_factory(f)

    print("Function", func_name)
    instrs = list(filter(lambda i: "op" in i, f["instrs"]))
    for i in instrs:
        state = analysis.get_state_before_instr(i)
        print("\t", state, sep="")
    if len(instrs) > 0:
        state = analysis.get_state_after_instr(instrs[-1])
        print("\t", state, sep="")

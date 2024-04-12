import bril_utils.bb
from bril_utils.fixed_point import run_to_converge


class LocalDCE:
    @run_to_converge
    def run(self, func) -> bool:
        insts = func["instrs"]
        self._blocks = bril_utils.bb.collect_blocks(func["instrs"])

        ret = False
        for blk in reversed(self._blocks):
            blk_begin_idx = blk.first_inst_idx
            blk_end_idx = blk.first_inst_idx + len(blk.insts)

            blk_changed = self._run_on_block(blk)
            ret = ret or blk_changed

            if blk_changed:
                insts[blk_begin_idx:blk_end_idx] = blk.insts

        return ret

    def _run_on_block(self, block: bril_utils.bb.Block) -> bool:
        # Collect all the variables that are used in the same function but outside of the block.
        # Definitions of these variables cannot be optimized away.
        used_vars = set()
        for func_block in self._blocks:
            if block is func_block:
                continue
            for inst in func_block.insts:
                if "args" in inst:
                    used_vars.update(inst["args"])

        @run_to_converge
        def _optimize() -> bool:
            # Indexes of the instructions that is useless.
            remove_inst_idx = []

            # Maintain a map from currently defined but unused variables to the index of the
            # instructions that define them.
            unused_vars = {}
            for (idx, inst) in enumerate(block.insts):
                # Remove all variables used by this instruction from the mapping.
                if "args" in inst:
                    for arg in inst["args"]:
                        unused_vars.pop(arg, None)

                # Add the variable defined by this instruction to the mapping.
                if "dest" in inst:
                    dest = inst["dest"]

                    # Before adding, if the variable is already in the mapping, add the previous
                    # instruction that defines the same variable to the to-remove list.
                    if dest in unused_vars:
                        remove_inst_idx.append(unused_vars[dest])

                    # Note that we cannot add the variable to the unused list if it's used somewhere
                    # else in the other blocks of the function.
                    if dest not in used_vars:
                        unused_vars[dest] = idx

            # We have reached the end of the basic block. All the variables that are still in the
            # mapping are also useless.
            remove_inst_idx.extend(unused_vars.values())

            # Finally, remove the recorded instructions from the block.
            for idx in remove_inst_idx[::-1]:
                block.insts.pop(idx)

            return len(remove_inst_idx) > 0

        return _optimize()

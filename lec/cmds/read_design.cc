#include "kernel/yosys.h"

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

struct ReadDesignPass : public Pass {
	ReadDesignPass() : Pass("read_design", "") { }
	void help() override
	{
		//   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
		log("\n");
		log("    chtype [options] [selection]\n");
		log("\n");
		log("Change the types of cells in the design.\n");
		log("\n");
		log("    -set <type>\n");
		log("        set the cell type to the given type\n");
		log("\n");
		log("    -map <old_type> <new_type>\n");
		log("        change cells types that match <old_type> to <new_type>\n");
		log("\n");
		log("\n");
	}
	void execute(std::vector<std::string> args, RTLIL::Design *design) override
	{
		IdString set_type;
		dict<IdString, IdString> map_types;

		size_t argidx;
		for (argidx = 1; argidx < args.size(); argidx++)
		{
			if (set_type == IdString() && args[argidx] == "-set" && argidx+1 < args.size()) {
				set_type = RTLIL::escape_id(args[++argidx]);
				continue;
			}
			if (args[argidx] == "-map" && argidx+2 < args.size()) {
				IdString old_type = RTLIL::escape_id(args[++argidx]);
				IdString new_type = RTLIL::escape_id(args[++argidx]);
				map_types[old_type] = new_type;
				continue;
			}
			break;
		}
		extra_args(args, argidx, design);

		for (auto module : design->selected_modules())
		{
			for (auto cell : module->selected_cells())
			{
				if (map_types.count(cell->type)) {
					cell->type = map_types.at(cell->type);
					continue;
				}

				if (set_type != IdString()) {
					cell->type = set_type;
					continue;
				}
			}
		}
	}
} ReadDesignPass;

PRIVATE_NAMESPACE_END

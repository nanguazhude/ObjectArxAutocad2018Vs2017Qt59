#include "object_arx_global.hpp"
#include "ARXApplication.hpp"

namespace sstd {

	extern void loadHellowWord();
	extern void loadSimpleDrawLine();
	extern void loadSimpleDrawLineChangeColor();
	extern void loadSimpleDrawPolygon();
	extern void loadPrintALlLayerName();
	extern void loadUpdateBlockFromOtherFile();
	extern void loadTestCode();
	extern void loadUpdateLayer();
	extern void loadUpdateTextStyle();

	namespace {
		using FunctionType = void(*)(void);
		static constexpr FunctionType _v_functions[] = {
			{ &sstd::loadHellowWord },
			{ &sstd::loadSimpleDrawLine },
			{ &sstd::loadSimpleDrawLineChangeColor },
			{ &sstd::loadSimpleDrawPolygon },
			{ &sstd::loadPrintALlLayerName },
			{ &sstd::loadUpdateBlockFromOtherFile },
			{ &sstd::loadTestCode },
			{ &sstd::loadUpdateLayer },
			{ &sstd::loadUpdateTextStyle },
		};
	}

	void ARXApplication::load() {
		for (const auto & varI : _v_functions) {
			(varI)();
		}
	}

	void ARXApplication::unload() {
		acedRegCmds->removeGroup(arx_group_name());
	}

	ARXApplication::ARXApplication() {

	}

}/*namespace sstd*/

/********************************/


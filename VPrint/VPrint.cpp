/* 如果不存在Layout则创建Layout , 否则不修改任何元素 */
#include "VPrint.hpp"
#include <string>

void sstd::VPrint::load() {
	arx_add_main_command<VPrint>();
}

namespace {
	class ThisState {
	public:
		std::wstring $LayoutName;
	};
}/*namespace*/

static inline void construct(ThisState * ) {}

static inline void getLayoutName(ThisState *) {}

static inline bool hasLayout(ThisState *) {}

static inline void createLayout(ThisState *) {}

void sstd::VPrint::main() try{
	auto varState = std::make_unique<ThisState>();
	construct(varState.get());
	getLayoutName(varState.get());
	if (hasLayout(varState.get())) { 
		return;
	}

}
catch (...) {}

/********************************/
namespace sstd {
	extern void loadVPrint() {
		VPrint::load();
	}
}

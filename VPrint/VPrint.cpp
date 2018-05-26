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
		AcDbDatabase *$DB;
	};
}/*namespace*/

////////////////////////
static inline void construct(ThisState * s) {
	s->$DB = acdbHostApplicationServices()->workingDatabase();
}

////////////////////////
static inline void getLayoutName(ThisState *s) {
	AcString varLayoutName;
	if (eOk != acedGetString(true/*允许包含空格*/, LR"(输入LayoutName:
)", varLayoutName)) {
		svthrow(LR"(can not get layout name)"sv);
	}
	varLayoutName.trim()/*remove space begin/end */;
	s->$LayoutName = std::wstring(
		static_cast<const wchar_t *>(varLayoutName),
		varLayoutName.length());
	if (s->$LayoutName.empty()) {
		svthrow(LR"(layout name is null)"sv);
	}
}

////////////////////////
static inline bool hasLayout(ThisState * s) {
	//AcDbLayout
	AcDbLayerStateManager * sm = s->$DB->getLayerStateManager();
	return false;
}

////////////////////////
static inline void createLayout(ThisState *) {

}

////////////////////////
void sstd::VPrint::main() try{
	auto varState = std::make_unique<ThisState>();
	construct(varState.get());
	getLayoutName(varState.get());
	if (hasLayout(varState.get())) { 
		/*如果已经有此name的layout,那么什么也不做...*/
		return;
	}
	createLayout(varState.get());
}
catch (...) {}

/********************************/
namespace sstd {
	extern void loadVPrint() {
		VPrint::load();
	}
}

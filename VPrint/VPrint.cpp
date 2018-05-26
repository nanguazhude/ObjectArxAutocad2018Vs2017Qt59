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
		AcDbLayerStateManager *$LM;
	};
}/*namespace*/

////////////////////////
static inline void construct(ThisState * s) {
	s->$DB = acdbHostApplicationServices()->workingDatabase();
	s->$LM = s->$DB->getLayerStateManager();
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

//http://adndevblog.typepad.com/autocad/2012/05/listing-the-layout-names.html
////////////////////////
static inline bool hasLayout(ThisState * s) {
	//AcDbLayout
	//AcDbLayerStateManager * sm = s->$DB->getLayerStateManager();
	auto & DB = s->$DB;
	sstd::ArxClosePointer<AcDbDictionary > varLayouts;
	if (eOk != DB->getLayoutDictionary(varLayouts.pointer(), kForRead))
		svthrow(LR"(can not find layouts)"sv);
	return varLayouts->has(s->$LayoutName.c_str());
	/*std::unique_ptr<AcDbDictionaryIterator> varIt{ varLayouts->newIterator() };
	if (nullptr == varIt) { svthrow(LR"(迭代失败)"sv); }
	for (;!varIt->done();varIt->next()) {

	}*/
	//return false;
}

//http://adndevblog.typepad.com/autocad/2012/06/create-a-layout-using-objectarx.html
////////////////////////
static inline void createLayout(ThisState *s) {
	auto LM = dynamic_cast<AcApLayoutManager *>(s->$LM);
	if (LM == nullptr) { svthrow(LR"(can not find layout manager)"sv); }
	AcDbObjectId layoutId;
	AcDbObjectId blockTableRecId;
	if(eOk!= LM->createLayout(s->$LayoutName.c_str(), layoutId, blockTableRecId))
		svthrow(LR"(can not create layout)"sv);
	//AcDbViewport
}

////////////////////////
void sstd::VPrint::main() try {
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

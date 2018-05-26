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
		AcDbLayoutManager* $_LM;
		AcApLayoutManager * $LM;
		AcDbObjectId $layoutId;
		AcDbObjectId $blockTableRecId;
	};
}/*namespace*/

////////////////////////
static inline void construct(ThisState * s) {
	s->$DB = acdbHostApplicationServices()->workingDatabase();
	s->$_LM = acdbHostApplicationServices()->layoutManager();
	s->$LM = dynamic_cast<AcApLayoutManager *>(s->$_LM);
	if (s->$LM == nullptr) { svthrow(LR"(can not find layout manager)"sv); }
}

////////////////////////
static inline void getLayoutName(ThisState *s) {
	AcString varLayoutName;
	if (RTNORM != acedGetString(true/*允许包含空格*/, LR"(输入LayoutName:
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
	if (eOk != s->$LM->createLayout(s->$LayoutName.c_str(), s->$layoutId, s->$blockTableRecId))
		svthrow(LR"(can not create layout)"sv);
	/*用默认打印机初始化*/
	s->$LM->setDefaultPlotConfig(s->$blockTableRecId);
	/*设置当前Layout*/
	s->$LM->setCurrentLayout(s->$LayoutName.c_str());
	/*更新状态栏*/
	s->$LM->updateLayoutTabs();
}

static inline AcDbObjectId current_viewport_id(ThisState *s) {
	AcDbObjectIdArray varViewPorts;
	sstd::ArxClosePointer< AcDbLayout > varLayout;
	if (eOk != acdbOpenObject(varLayout.pointer(), s->$layoutId, kForWrite))
		svthrow(LR"(can not open layout)");
	varViewPorts = varLayout->getViewportArray();
	if (varViewPorts.length() < 2)
		svthrow(LR"(can not find viewport)");
	return varViewPorts[1];
}

static inline void setplotstyle(ThisState *s) {

	bool varFlageK;
	{
		AcDbObjectId mCurViewportId = current_viewport_id(s);
		sstd::ArxClosePointer<AcDbViewport> pCurViewport;
		if (Acad::eOk != acdbOpenObject(pCurViewport.pointer(), mCurViewportId,
			AcDb::kForWrite)) {
			svthrow(LR"(can not open current view port)"sv);
		}

		varFlageK = pCurViewport->height() > pCurViewport->width();
	}

	AcDbLayout *  pLayout = nullptr;
	if (eOk != acdbOpenObject(pLayout, s->$layoutId, AcDb::kForWrite))
		svthrow(LR"(can not open layout)");

	class PLayoutLock {
		AcDbLayout * d;
	public:
		PLayoutLock(AcDbLayout *a) :d(a) {}
		~PLayoutLock() { d->close(); }
	}PlayoutLock(pLayout);

	//AcDbPlotSettingsValidator
	AcDbPlotSettingsValidator * pPlotSettingsValidator = acdbHostApplicationServices()->plotSettingsValidator();
	if (pPlotSettingsValidator == nullptr) { svthrow(LR"(pointer is null)"sv); }
	//plot dialog box
	pPlotSettingsValidator->refreshLists(pLayout);	// Refresh the layout lists in order to use it
													//
													//pPlotSettingsValidator->setPlotViewName(pLayout, LR"(my_plot_view)");
													//pPlotSettingsValidator->setPlotViewName(pLayout,LR"()");
													//@AutoCAD PDF (High Quality Print).pc3
	auto es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF(High Quality Print).pc3)");	//设置打印设备
	if (es != eOk) {
		es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF (High Quality Print).pc3)");	//设置打印设备
		if (es != eOk) {
			svthrow(LR"(无法找到:@AutoCAD PDF (High Quality Print).pc3)"sv);
		}
	}


	//es = pPlotSettingsValidator->setCanonicalMediaName(pLayout, LR"(User 1)");//设置图纸尺寸(A4?)

	{
		const auto globalPlostStyle = s->$DB->plotStyleMode();
		if (globalPlostStyle) {/*color*/
			es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout, LR"(monochrome.ctb)");//设置打印样式表
		}
		else {/*not color*/
			es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout, LR"(monochrome.stb)");//设置打印样式表
		}
	}
	//es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout,_T("JSTRI.ctb"));//设置打印样式表
	//https://forums.autodesk.com/t5/net/setplotwindowarea-got-an-offset/m-p/6341629 /**/
	//es = pPlotSettingsValidator->setPlotWindowArea(pLayout, x0, y0, x1, y1); //设置打印范围
	//es = pPlotSettingsValidator->setPlotOrigin(pLayout, x0, y0);	//设置origin
	es = pPlotSettingsValidator->setPlotCentered(pLayout, true);	//是否居中打印
	es = pPlotSettingsValidator->setPlotType(pLayout, AcDbPlotSettings::kWindow);	//打印类型
																					//es = pPlotSettingsValidator->setStdScale(pPlotSettings, dbScale);	//比例
	if (varFlageK/*y>x?*/) {
		es = pPlotSettingsValidator->setPlotRotation(pLayout, AcDbPlotSettings::k90degrees);//设置打印方向
	}
	else {
		es = pPlotSettingsValidator->setPlotRotation(pLayout, AcDbPlotSettings::k0degrees);//设置打印方向
	}

	es = pPlotSettingsValidator->setStdScaleType(pLayout, AcDbPlotSettings::StdScaleType::kScaleToFit);//布满图纸

}

//https://forums.autodesk.com/t5/net/layouts-and-viewports/td-p/3128748
//http://through-the-interface.typepad.com/through_the_interface/2015/11/creating-an-autocad-layout-with-custom-plot-and-viewport-settings-using-net.html
//////////////////////////////////////
static inline void setviewport(ThisState *s) {
	AcDbObjectId mCurViewportId = current_viewport_id(s);

	sstd::ArxClosePointer<AcDbViewport> pCurViewport;
	if (Acad::eOk != acdbOpenObject(pCurViewport.pointer(), mCurViewportId,
		AcDb::kForWrite)) {
		svthrow(LR"(can not open current view port)"sv);
	}

	pCurViewport->setLayer(LR"(不打印)");
	pCurViewport->setWidth(200);
	pCurViewport->setHeight(200);
	pCurViewport->setCenterPoint({ 1,2,0 });

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
	setplotstyle(varState.get());
	setviewport(varState.get());
}
catch (...) {}

/********************************/
namespace sstd {
	extern void loadVPrint() {
		VPrint::load();
	}
}

///////////////////////////
/* A4纸尺寸:210×297 */

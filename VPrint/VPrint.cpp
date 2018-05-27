/* 如果不存在Layout则创建Layout , 否则不修改任何元素 */
#include "VPrint.hpp"
#include <string>
#include <set>
#include <vector>
namespace sstd {
	extern void WCS2DCS(const double * i, double * o);
}

static inline void refreshDisplay() {
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

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
		AcGePoint3d $ModelMin;
		AcGePoint3d $ModelMax;
		bool $isYmax;
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
	if (eOk != s->$LM->createLayout(s->$LayoutName.c_str(),
		s->$layoutId, 
		s->$blockTableRecId))
		svthrow(LR"(can not create layout)"sv);
	/*用默认打印机初始化*/
	//s->$LM->setDefaultPlotConfig(s->$blockTableRecId);
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

	AcDbLayout * pLayout = nullptr;
	/***************************************************************/
	///try to find current layout 
	if constexpr(true) {
		auto * pLayoutManager = acdbHostApplicationServices()->layoutManager();
		AcDbObjectId idBTR = pLayoutManager->getActiveLayoutBTRId();
		sstd::ArxClosePointer< AcDbBlockTableRecord  >pBTR;
		auto es = acdbOpenObject(pBTR.pointer(), idBTR, AcDb::kForRead);
		if (es != Acad::eOk) { svthrow(LR"(can not open )"sv) ; }
		AcDbObjectId idLayout = pBTR->getLayoutId();
		es = acdbOpenObject(pLayout, idLayout, AcDb::kForWrite);
		if (es != Acad::eOk) { svthrow(LR"(can not open )"sv); }
	}
	/***************************************************************/
	else {
		if (eOk != acdbOpenObject(pLayout, s->$layoutId, AcDb::kForWrite))
			svthrow(LR"(can not open layout)");
	}

	//std::unique_ptr<AcDbPlotSettings> ps{ new AcDbPlotSettings(pLayout->modelType() )};

	class PLayoutLock {
		AcDbLayout * d;
	public:
		PLayoutLock(AcDbLayout *a) :d(a) {}
		~PLayoutLock() { d->close(); }
	}PlayoutLock(pLayout);

	{
		varFlageK = std::abs(s->$ModelMax.y/**/ -/**/s->$ModelMin.y) >
			std::abs(s->$ModelMax.x/**/ -/**/s->$ModelMin.x);
		s->$isYmax = varFlageK;
	}

	//AcDbPlotSettingsValidator
	AcDbPlotSettingsValidator * pPlotSettingsValidator = acdbHostApplicationServices()->plotSettingsValidator();
	if (pPlotSettingsValidator == nullptr) { svthrow(LR"(pointer is null)"sv); }
	//ps->copyFrom(pLayout);

	//plot dialog box
	//pPlotSettingsValidator->refreshLists(pLayout);	// Refresh the layout lists in order to use it
													//
	//pPlotSettingsValidator->setPlotViewName(pLayout, s->$LayoutName.c_str());
	auto es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF(High Quality Print).pc3)");	//设置打印设备
	if (es != eOk) {
		es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF (High Quality Print).pc3)");	//设置打印设备
		if (es != eOk) {
			svthrow(LR"(无法找到:@AutoCAD PDF (High Quality Print).pc3)"sv);
		}
	}
	pPlotSettingsValidator->refreshLists(pLayout);

	{
		AcArray<const ACHAR *>  mediaList;
		pPlotSettingsValidator->canonicalMediaNameList(pLayout, mediaList);
		if (mediaList.length() < 1) { svthrow(LR"(can not find canonicalMediaNameList)"); }
		const auto varPageName = mediaList[0];
		es = pPlotSettingsValidator->setCanonicalMediaName(pLayout, varPageName);//设置图纸尺寸(A4?)
		/*if (es != eOk) {
			svthrow(LR"(setCanonicalMediaName)"sv);
		}*/
	}

	//pPlotSettingsValidator->setPlotPaperUnits(pLayout, AcDbPlotSettings::kMillimeters);

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
	//es = pPlotSettingsValidator->setPlotCentered(pLayout, true);	//是否居中打印
	//es = pPlotSettingsValidator->setPlotType(pLayout, AcDbPlotSettings::kLayout);	//打印类型
	//es = pPlotSettingsValidator->setStdScale(pPlotSettings, dbScale);	//比例
	if (varFlageK/*y>x?*/) {
		es = pPlotSettingsValidator->setPlotRotation(pLayout, AcDbPlotSettings::k90degrees);//设置打印方向
	}
	else {
		es = pPlotSettingsValidator->setPlotRotation(pLayout, AcDbPlotSettings::k0degrees);//设置打印方向
	}

	es = pPlotSettingsValidator->setPlotOrigin(pLayout, 0, 0);
	
}


//http://help.autodesk.com/view/OARX/2019/ESP/?guid=GUID-4B512161-DBD4-43DA-BD89-AA2EA564F9F9
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

	double varWidth;
	double varHeight;
	//double printableXmin;
	//double printableYmin;
	//double printableXmax;
	//double printableYmax;
	{
		AcDbLayout * pLayout = nullptr;
		if (eOk != acdbOpenObject(pLayout, s->$layoutId, AcDb::kForWrite))
			svthrow(LR"(can not open layout)");
		class PLayoutLock {
			AcDbLayout * d;
		public:
			PLayoutLock(AcDbLayout *a) :d(a) {}
			~PLayoutLock() { d->close(); }
		}PlayoutLock(pLayout);
		pLayout->getPlotPaperSize(varWidth, varHeight);
		/*if width heigth 与 x y 概念不一致,进行修正*/
		/*if (!((varHeight>varWidth)&&(s->$isYmax))) {
			std::swap(varWidth, varHeight);
		}*/
		//std::swap(varWidth, varHeight);
		//pLayout->getPlotPaperMargins(printableXmin, printableYmin, printableXmax, printableYmax);
	}

	/*无边框打印*/
	pCurViewport->setLayer(LR"(不打印)");
	pCurViewport->setWidth(varWidth);
	pCurViewport->setHeight(varHeight);
	pCurViewport->setCenterPoint({ varWidth*0.5,varHeight*0.5, 0. });
	pCurViewport->recordGraphicsModified(true);

	/********************/
	AcGePoint3d varCP = s->$ModelMax;
	varCP.x *= 0.5;
	varCP.y *= 0.5;
	varCP.x = std::fma(s->$ModelMin.x, 0.5, varCP.x);
	varCP.y = std::fma(s->$ModelMin.y, 0.5, varCP.y);
	varCP.z = 0;
	//sstd::WCS2DCS(&varCP.x, &varCP.x);

	pCurViewport->setViewCenter({ 0 ,0 });
	pCurViewport->setViewTarget(varCP);
	pCurViewport->setViewDirection({ 0,0,1 });
	pCurViewport->setCustomScale(std::min(
		varWidth / std::abs(s->$ModelMax.x - s->$ModelMin.x),
		varHeight / std::abs(s->$ModelMax.y - s->$ModelMin.y)));
	pCurViewport->setLocked();

}

///在模型空间中找固定块的名称
static inline std::vector<AcDbObjectId> _p_getModelSize(ThisState *s) {
	const static std::wstring varName{ LR"(横边框2(G3000))"sv };
	auto & $DB = s->$DB;

	std::set< AcDbObjectId > varBID;
	std::set< AcDbObjectId > varEBID;
	{
		sstd::ArxClosePointer< AcDbBlockTable> varBlockTable;
		if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			svthrow(LR"(获得AcDbBlockTable失败1)"sv);
		}

		sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
		if (eOk != varBlockTable->getAt(varName.c_str(),
			varR.pointer(),
			AcDb::kForRead)) {
			svthrow(LR"(获得AcDbBlockTableRecord失败2)"sv);
		}

		/* 如果只有一个非匿名对象,则返回它 */
		//std::optional< AcDbObjectId > varJustReturnOneID;
		AcDbObjectIdArray varIDS_;
		AcDbObjectIdArray varEIDS_;
		{
			AcDbDynBlockTableRecord varDR_{ varR->objectId() };
			if (varDR_.isDynamicBlock()) {
				AcDbObjectIdArray varBLKIDS_;
				//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR->getErasedBlockReferenceIds(varEIDS_);
				varR.close();
				//if (varIDS_.length() == 1) {
				//	varJustReturnOneID = *varIDS_.begin();
				//}
				/*尝试从动态块获得ID*/
				varDR_.getAnonymousBlockIds(varBLKIDS_);
				//acutPrintf(L"---%d",varBLKIDS_.length());
				for (const auto & varJ : varBLKIDS_) {
					sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
					if (eOk != acdbOpenObject(varR.pointer(), varJ)) {
						continue;
					}
					AcDbObjectIdArray varTmpIDS;
					AcDbObjectIdArray varTmpEIDS;
					varR->getBlockReferenceIds(varTmpIDS);
					varR->getErasedBlockReferenceIds(varTmpEIDS);
					varIDS_.append(varTmpIDS);
					varEIDS_.append(varTmpEIDS);
				}
			}
			else {//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR->getErasedBlockReferenceIds(varEIDS_);
				varR.close();
			}
		}

		for (const auto & varJ : varIDS_) {
			sstd::ArxClosePointer<AcDbBlockReference> varBR;
			if (eOk != acdbOpenObject(varBR.pointer(), varJ, AcDb::kForWrite)) {
				continue;
			}
			varBID.insert(varBR->objectId());
		}

		if (varBID.empty()) {
			svthrow(LR"(can find block)"sv);
		}

		for (const auto & varJ : varEIDS_) {
			varEBID.insert(varJ);
		}

		//		if (varJustReturnOneID) {
		//			return { static_cast<std::size_t>(1) , *varJustReturnOneID };
		//		}
	}

	if (varBID.empty()) { svthrow(LR"(empty)"); }
	if (varBID.size() == 1) { return { 1,*varBID.begin() }; }

	if constexpr(true) {
		std::vector< AcDbObjectId > varAns;
		varAns.reserve(varBID.size());
		for (const auto & varI : varBID) {
			sstd::ArxClosePointer< AcDbBlockReference > varR;
			if (eOk != acdbOpenObject(varR.pointer(), varI)) {
				continue/*忽略无法打开的元素*/;
			}
			//if (varR->isEraseStatusToggled()) { continue; }
			if (varEBID.count(varI) > 0) { continue/*忽略被删除对象*/; }
			//if (varR->isErased()) { continue/*忽略被删除的对象*/; }
			//if (varR->visibility() != kVisible) { continue/*忽略不可见对象*/; }
			class HighLightLock {
				AcDbBlockReference * d;
			public:
				HighLightLock(AcDbBlockReference*a) :d(a) { d->highlight(); }
				~HighLightLock() { d->unhighlight(); }
			}varLock{ varR.pointer() };
			AcString varKey;
			acedGetString(false, LR"(只打印此元素?[Yes]<N>)", varKey);
			if ((varKey.isEmpty() == false) && ((varKey[0] == 'Y') || (varKey[0] == 'y'))) {
				return { 1,varI };
			}
			varAns.push_back(varI);
		}
		return std::move(varAns);
	}/*constexpr*/
	else {
		return { varBID.begin(),varBID.end() };
	}

}

static inline void getModelSize(ThisState *s) {
	const auto varItems = _p_getModelSize(s);
	if (varItems.size() != 1) {
		svthrow(LR"(块数量错误!)"sv);
	}
	sstd::ArxClosePointer< AcDbEntity > varItem;
	if (eOk != acdbOpenObject(varItem.pointer(), varItems[0], kForRead))
		svthrow(LR"(can not open object)"sv);
	AcDbExtents varTmp;
	varItem->bounds(varTmp);
	s->$ModelMin = varTmp.minPoint();
	s->$ModelMax = varTmp.maxPoint();
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
	getModelSize(varState.get());
	createLayout(varState.get());
	setplotstyle(varState.get());
	setviewport(varState.get());
	//refreshDisplay();
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
//1071
//864



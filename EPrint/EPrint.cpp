#include "EPrint.hpp"
#include <acedCmdNF.h>
#include <string>
#include <memory>
#include <set>
#include <string_view>

using namespace std::string_view_literals;
extern AcPlPlotProgressDialog * _sstd_acplCreatePlotProgressDialog();

//AcDbDynBlockReference
namespace {
	class wstring :public std::wstring {
		using super = std::wstring;
	public:
		wstring() = default;
		wstring(const std::wstring_view & arg) :super(arg.data(), arg.size()) {}
	};
}/*namespace*/

namespace sstd {

	extern void loadEPrint() {
		EPrint::load();
	}

	void EPrint::load() {
		arx_add_main_command<EPrint>();
	}

	////////////////////////////////////////
	void print_all_used_block_name(AcDbDatabase*$DB) {
		std::vector< AcDbObjectId > varBTIDS;
		{
			sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
			if (Acad::eOk == $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
				std::unique_ptr<AcDbBlockTableIterator> varIt;
				{
					AcDbBlockTableIterator * varTmp = nullptr;
					if (Acad::eOk != varBlockTable->newIterator(varTmp)) {
						return;
					}
					varIt.reset(varTmp);
				}

				for (; !varIt->done(); varIt->step()) {
					sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
					if (Acad::eOk != varIt->getRecord(varR)) {
						continue;
					}

					{
						AcDbBlockTableRecord * varT = varR;
						std::unique_ptr<AcDbBlockTableRecordIterator> varIt;
						{
							AcDbBlockTableRecordIterator *varTmp;
							if (varT->newIterator(varTmp) != Acad::eOk) { return; }
							varIt.reset(varTmp);
						}

						for (; !varIt->done(); varIt->step()) {
							sstd::ArxClosePointer< AcDbEntity> varEnt;
							if (varIt->getEntity(varEnt, AcDb::kForRead) != Acad::eOk) {
								continue;
							}

							AcDbBlockReference * varBR = AcDbBlockReference::cast(varEnt.pointer());


							if (varBR) {
								AcDbDynBlockReference varDyID{ varBR->objectId() };
								if (varDyID.isDynamicBlock()) {
									varBTIDS.push_back(varDyID.dynamicBlockTableRecord());
								}
								else {
									varBTIDS.push_back(varBR->blockTableRecord());
								}

							}
							/*********************************/
							if (varBR) {
								std::unique_ptr< AcDbObjectIterator> varAIt{
									varBR->attributeIterator()
								};

								for (; !varAIt->done(); varAIt->step()) {
									sstd::ArxClosePointer< AcDbAttribute > varAT;
									if (eOk != acdbOpenObject(varAT.pointer(),
										varAIt->objectId(),
										AcDb::kForWrite)) {
										acutPrintf(LR"(Can not open AcDbAttribute!
)");
										continue;
									}

									const sstd::ArxString varTag = varAT->tag();
									acutPrintf(varTag);
									acutPrintf(LR"(:
)");
								}

							}
							/**********************************/
						}
					}

				}

			}

			for (const auto & varI : varBTIDS) {
				//AcDbBlockTableRecord  * cc;
				sstd::ArxClosePointer<AcDbBlockTableRecord > pBlkTblRec;
				if (eOk != acdbOpenObject(pBlkTblRec.pointer(), varI, AcDb::kForRead)) {
					continue;
				}

				AcString varName;
				pBlkTblRec->getName(varName);
				acutPrintf(LR"(---
)");
				acutPrintf(varName);


				//pBlkTblRec->setAttributes();
			}

		}

	}
	////////////////////////////////////////

	/*set block up*/
	template<bool ToUp = true>
	std::vector<AcDbObjectId > setBlockDrawOrder(
		AcDbDatabase*$DB,
		const wstring&varName) {

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

		AcDbObjectIdArray varIDS_;
		{
			AcDbDynBlockTableRecord varDR_{ varR->objectId() };
			if (varDR_.isDynamicBlock()) {
				AcDbObjectIdArray varBLKIDS_;
				//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR.close();
				/*尝试从动态块获得ID*/
				varDR_.getAnonymousBlockIds(varBLKIDS_);
				//acutPrintf(L"---%d",varBLKIDS_.length());
				for (const auto & varJ : varBLKIDS_) {
					sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
					if (eOk != acdbOpenObject(varR.pointer(), varJ)) {
						continue;
					}
					AcDbObjectIdArray varTmpIDS;
					varR->getBlockReferenceIds(varTmpIDS);
					varIDS_.append(varTmpIDS);
				}
			}
			else {//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR.close();
			}
		}

		std::set< AcDbObjectId > varBID;
		for (const auto & varJ : varIDS_) {

			sstd::ArxClosePointer<AcDbBlockReference> varBR;
			if (eOk != acdbOpenObject(varBR.pointer(), varJ, AcDb::kForWrite)) {
				continue;
			}

			varBID.insert(varBR->objectId());
			varBR->setLayer(LR"(尺寸线)");

		}

		if (varBID.empty()) { return{}; }

		{
			class Lock {
			public:
				ads_name ss = {};
				Lock() { construct(); }
				void construct() { acedSSAdd(nullptr, nullptr, ss); }
				void destory() { acedSSFree(ss); ss[0] = 0; ss[1] = 0; }
				~Lock() { destory(); }
			} varLock;

			for (const auto & varI : varBID) {
				ads_name s;
				acdbGetAdsName(s, varI);
				acedSSAdd(s, varLock.ss, varLock.ss);
			}

			if constexpr(ToUp) {
				acedCommandS(
					RTSTR, LR"(DRAWORDER)",
					RTPICKS, varLock.ss,
					RTSTR, L"",
					RTSTR, L"F",
					RTNONE);
			}
			else {
				acedCommandS(
					RTSTR, LR"(DRAWORDER)",
					RTPICKS, varLock.ss,
					RTSTR, L"",
					RTSTR, L"B",
					RTNONE);
			}

		}

		return{ varBID.begin(),varBID.end() };
	}

	bool _setPlotArea(
		const double x0, const double y0,
		const double x1, const double y1,
		const wstring & strFileName) {
		/*https://forums.autodesk.com/t5/objectarx/problem-of-plotting/m-p/6803724 */
		/*https://www.cnblogs.com/c-gis/articles/2214470.html */
		Acad::ErrorStatus es;

		class Locker {
		public:
			AcPlPlotEngine * PlotEngine = nullptr;
			AcDbBlockTableRecord *pBTR = nullptr;
			AcDbLayout *pLayout = nullptr;
			AcDbPlotSettings * pPlotSettings = nullptr;
			AcPlPlotProgressDialog * pProgress = nullptr;
			~Locker() {
				if (PlotEngine)PlotEngine->destroy();
				if (pBTR) { pBTR->close(); }
				if (pLayout) { pLayout->close(); }
				if (pPlotSettings) { delete pPlotSettings; }
				if (pProgress) { pProgress->destroy(); }
			}
		}varData;

		AcPlPlotEngine * & PlotEngine = varData.PlotEngine;
		if (Acad::eOk != AcPlPlotFactory::createPublishEngine(PlotEngine)) return false;

		//创建进度条
		AcPlPlotProgressDialog * & pProgress = varData.pProgress;
		pProgress = _sstd_acplCreatePlotProgressDialog();
		if (pProgress) {
			const wstring varTmp = wstring(LR"(打印进度 - )"sv) + strFileName;
			pProgress->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle, varTmp.c_str());
			pProgress->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg, LR"(取消打印)");
			pProgress->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg, LR"(取消作业)");
			pProgress->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption, LR"(打印进度)");
			pProgress->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption, LR"(打印进度)");
			pProgress->setPlotProgressRange(0, 100);
			pProgress->onBeginPlot();
			pProgress->setIsVisible(true);
		}

		// get a pointer to the layout manager
		auto * pLayoutManager = acdbHostApplicationServices()->layoutManager();
		AcDbBlockTableRecord * & pBTR = varData.pBTR;
		{
			AcDbObjectId idBTR = pLayoutManager->getActiveLayoutBTRId();
			es = acdbOpenObject(pBTR, idBTR, AcDb::kForRead);
			if (es != Acad::eOk) { return false; }
		}

		// We want to instantiate our custom AcDbPlotSettings object, and inherit the 
		// properties from the active layout.
		AcDbLayout * & pLayout = varData.pLayout;
		AcDbObjectId idLayout = pBTR->getLayoutId();
		{
			es = acdbOpenObject(pLayout, idLayout, AcDb::kForRead);
			if (es != Acad::eOk) { return false; }
		}

		AcDbPlotSettings * & pPlotSettings = varData.pPlotSettings;
		{
			pPlotSettings = new AcDbPlotSettings(pLayout->modelType());
			es = pPlotSettings->copyFrom(pLayout);
			if (es != Acad::eOk) { return false; }
		}

		{
			pLayout->close();
			pLayout = nullptr;
			pBTR->close();
			pBTR = nullptr;
		}

		auto * pPlotSettingsValidator = acdbHostApplicationServices()->plotSettingsValidator();
		if (pPlotSettingsValidator == nullptr) return false;

		pPlotSettingsValidator->refreshLists(pPlotSettings);	// Refresh the layout lists in order to use it
		es = pPlotSettingsValidator->setPlotCfgName(pPlotSettings, LR"(@AutoCAD PDF(High Quality Print).pc3)");	//设置打印设备
		es = pPlotSettingsValidator->setCanonicalMediaName(pLayout, LR"(User 1)");//设置图纸尺寸
		//es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout,_T("JSTRI.ctb"));//设置打印样式表
		es = pPlotSettingsValidator->setPlotWindowArea(pPlotSettings, x0, y0, x1, y1); //设置打印范围
		es = pPlotSettingsValidator->setPlotOrigin(pPlotSettings, x0, y0);	//设置origin
		es = pPlotSettingsValidator->setPlotCentered(pPlotSettings, true);	//是否居中打印
		es = pPlotSettingsValidator->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);	//打印类型
		//es = pPlotSettingsValidator->setStdScale(pPlotSettings, dbScale);	//比例
		es = pPlotSettingsValidator->setPlotRotation(pLayout, AcDbPlotSettings::k0degrees);//设置打印方向
		es = pPlotSettingsValidator->setStdScaleType(pLayout, AcDbPlotSettings::StdScaleType::kScaleToFit);//布满图纸
		// we have to close the layout here because the last parameter of
		const struct PlotEngineLock {
			AcPlPlotEngine * & d;
			PlotEngineLock(AcPlPlotEngine *&a, AcPlPlotProgressDialog *  p, Acad::ErrorStatus&es) :d(a) {
				es = d->beginPlot(p, nullptr);
				if (es != eOk) { svthrow(LR"(can not beginPlot)"sv); }
			}
			~PlotEngineLock() { d->endPlot(); }
		}varPlotEngineLock(PlotEngine, pProgress, es);																								   // findLayoutNamed is true, leave layout open


		AcPlPlotInfo plotInfo;
		plotInfo.setLayout(idLayout);
		plotInfo.setOverrideSettings(pPlotSettings);

		if constexpr(false) {
			AcPlPlotConfig * pPlotConfig = nullptr;
			es = acplPlotConfigManagerPtr()->setCurrentConfig(pPlotConfig, LR"(@AutoCAD PDF(High Quality Print).pc3)");
			if (es != eOk) {
				svthrow(LR"(can not setCurrentConfig)"sv);
			}
			pPlotConfig->setPlotToFile(true);
			pPlotConfig->refreshMediaNameList();
			plotInfo.setDeviceOverride(pPlotConfig);
		}

		//开始打印
		AcPlPlotInfoValidator plotInfoValidator;
		AcPlPlotPageInfo pageInfo;
		plotInfoValidator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
		plotInfoValidator.validate(plotInfo);
		es = PlotEngine->beginDocument(plotInfo, acDocManager->curDocument()->fileName(), nullptr, 1, true, strFileName.c_str());
		if (es != eOk) { svthrow(LR"(can not beginDocument)"sv); }
		struct PlotEngineLock {
			AcPlPlotEngine * & d;
			PlotEngineLock(AcPlPlotEngine * &a) :d(a) {}
			~PlotEngineLock() { d->endDocument(); }
		}varPlotEngine(PlotEngine);

		es = PlotEngine->beginPage(pageInfo, plotInfo, true);
		if (es != eOk) { svthrow(LR"(can not beginPage)"sv); }
		struct PlotEngineLockPage {
			AcPlPlotEngine * & d;
			PlotEngineLockPage(AcPlPlotEngine * &a) :d(a) {}
			~PlotEngineLockPage() { d->endPage(); }
		}varPlotEnginePage(PlotEngine);

		es = PlotEngine->beginGenerateGraphics();
		if (es != eOk) { svthrow(LR"(can not GenerateGraphics)"sv); }
		struct PlotEngineLockGenerateGraphics {
			AcPlPlotEngine * & d;
			PlotEngineLockGenerateGraphics(AcPlPlotEngine * &a) :d(a) {}
			~PlotEngineLockGenerateGraphics() { d->endGenerateGraphics(); }
		}varPlotEngineLockGenerateGraphics(PlotEngine);

		return true;
	}

	bool setPlotArea(AcDbDatabase*, const AcDbObjectId & varHBKID) {
		AcGePoint3d varBottomLeft;
		AcGePoint3d varTopRight;
		wstring varFileName;
		/*获得打印文件名称*/
		{
			const wchar_t * varFileName1
				= acDocManager
				->mdiActiveDocument()
				->fileName();
			if (varFileName1 == nullptr) { return false; }
			const auto varFileNameQ = QString::fromWCharArray(varFileName1);

			QtApplication varQtApp;
			const auto varFileInfo = QFileInfo(varFileNameQ);
			const auto varFilePath = varFileInfo.absolutePath()/*获得完整路径*/;

			QDir varDirTmp{ varFilePath };
			varFileName.assign(varDirTmp.absoluteFilePath(varFileInfo.baseName() +
				QString::fromUtf8(u8R"(.uncheck.pdf)")).toStdWString());

		}
		/*获得打印区域*/
		{
			sstd::ArxClosePointer< AcDbBlockReference > varR;
			if (eOk != acdbOpenObject(varR.pointer(), varHBKID)) {
				svthrow(LR"(can not open  AcDbBlockReference)");
			}
			AcDbExtents varBound;
			varR->bounds(varBound);
			varTopRight = varBound.maxPoint();
			varBottomLeft = varBound.minPoint();
		}

		return _setPlotArea(
			varBottomLeft.x, varBottomLeft.y,
			varTopRight.x, varTopRight.y,
			varFileName);

	}

	void fitTextArea(
		const AcDbObjectId & varHBKID,
		const AcDbObjectId & varBTLID) {
		sstd::ArxClosePointer< AcDbBlockReference > varHBK;
		sstd::ArxClosePointer< AcDbBlockReference > varBTL;

		/*
		0,1 :
		2,3 :text
		*/
		AcGePoint3d varLimit[4];

		{
			if (eOk != acdbOpenObject(varHBK.pointer(), varHBKID)) {
				svthrow(LR"(can not open  AcDbBlockReference)");
			}
			AcDbExtents varBound;
			varHBK->bounds(varBound);
			varLimit[0] = varBound.maxPoint();
			varLimit[1] = varBound.minPoint();
		}

		{
			if (eOk != acdbOpenObject(varBTL.pointer(), varBTLID, AcDb::kForWrite)) {
				svthrow(LR"(can not open  AcDbBlockReference)");
			}
			AcDbExtents varBound;
			varBTL->bounds(varBound);
			varLimit[2] = varBound.maxPoint();
			varLimit[3] = varBound.minPoint();
		}

		/*top , bottom*/
		constexpr const double varTopDy = 112.178;
		constexpr const double varTotalDy = 599.31;
		constexpr const double varBottomDy = 17.87;
		constexpr const double varX_ = 17.16;
		const auto varK = std::abs(varLimit[0].y - varLimit[1].y) / varTotalDy;
		const double varTopY = std::fma(varTopDy, varK, varLimit[1].y);
		const double varBottomY = std::fma(varBottomDy, varK, varLimit[1].y);
		const double varX = std::fma(varX_, varK, varLimit[1].x);
		/*align*/
		{
			auto varSY = std::abs(varTopY - varBottomY) / std::abs(varLimit[3].y - varLimit[2].y);
			const auto varSFS = varBTL->scaleFactors();
			varSY *= varSFS.sx;
			varBTL->setScaleFactors({ varSY,varSY,varSY });
		}
		varBTL->setPosition({ varX,varTopY,0 });
	}

	void EPrint::main() try {
		auto DB = acdbHostApplicationServices()->workingDatabase();
		//print_all_used_block_name(DB);
		std::vector<AcDbObjectId> varBTLIDS;
		std::vector<AcDbObjectId> varHBKIDS;
		try {
			varBTLIDS = setBlockDrawOrder<false>(DB, LR"(@标题栏文字(1))"sv);
		}
		catch (...) {
			acutPrintf(LR"(e@标题栏文字(1)
)");
		}

		try {
			varHBKIDS = setBlockDrawOrder<true>(DB, LR"(横边框2(G3000))"sv);
		}
		catch (...) {
			acutPrintf(LR"(e@横边框2(G3000)
)");
		}

		/*调整标题栏*/
		if ((varBTLIDS.size() == 1) && (varHBKIDS.size() == 1)) {
			fitTextArea(varHBKIDS[0], varBTLIDS[0]);
		}

		/*设置打印区域,并打印*/
		if (varHBKIDS.size() == 1) {
			setPlotArea(DB, varHBKIDS[0]);
		}

	}
	catch (...) {}

}/*namespace sstd*/





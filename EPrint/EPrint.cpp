#include "EPrint.hpp"
#include <acedCmdNF.h>
#include <string>
#include <memory>
#include <set>
#include <string_view>
using namespace std::string_view_literals;
//AcDbDynBlockReference
namespace {
	class wstring :public std::wstring {
		using super = std::wstring;
	public:
		using super::super;
		wstring(const std::wstring_view & arg) :wstring(arg.data(), arg.size()) {}
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
			//尝试从动态块获得ID
			AcDbDynBlockTableRecord varDR_{ varR->objectId() };
			if (varDR_.isDynamicBlock()) {
				AcDbObjectIdArray varBLKIDS_;
				varDR_.getAnonymousBlockIds(varBLKIDS_);
				//acutPrintf(L"---%d",varBLKIDS_.length());
				varR.close();
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

	bool _setPlotArea(AcDbDatabase*$DB, const AcDbObjectId & varHBKID,
		const double x0,const double y0,
		const double x1,const double y1,
		const wstring strFileName) {
		/*https://forums.autodesk.com/t5/objectarx/problem-of-plotting/m-p/6803724*/
		Acad::ErrorStatus es;

		class Locker {
		public:
			AcPlPlotEngine * PlotEngine = nullptr;
			AcDbBlockTableRecord *pBTR = nullptr;
			AcDbLayout *pLayout = nullptr;
			AcDbPlotSettings * pPlotSettings = nullptr;
			~Locker() {
				if (PlotEngine)PlotEngine->destroy();
				if (pBTR) { pBTR->close(); }
				if (pLayout) { pLayout->close(); }
				if (pPlotSettings) { delete pPlotSettings; }
			}
		}varData;

		AcPlPlotEngine * & PlotEngine = varData.PlotEngine;
		if (Acad::eOk != AcPlPlotFactory::createPublishEngine(PlotEngine)) return false;

		// get a pointer to the layout manager
		AcApLayoutManager *pLayoutManager = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager();
		AcDbObjectId idBTR = pLayoutManager->getActiveLayoutBTRId();
		AcDbBlockTableRecord * & pBTR = varData.pBTR;

		es = acdbOpenObject(pBTR, idBTR, AcDb::kForRead);
		if (es != Acad::eOk) { return false; }

		// We want to instantiate our custom AcDbPlotSettings object, and inherit the 
		// properties from the active layout.
		AcDbObjectId idLayout = pBTR->getLayoutId();
		AcDbLayout * & pLayout = varData.pLayout;

		es = acdbOpenObject(pLayout, idLayout, AcDb::kForRead);
		if (es != Acad::eOk) { return false; }

		AcDbPlotSettings * & pPlotSettings =varData.pPlotSettings; 
		pPlotSettings = new AcDbPlotSettings(pLayout->modelType());
		es = pPlotSettings->copyFrom(pLayout);
		if (es != Acad::eOk) { return false; }

		pLayout->close();
		pLayout = nullptr;
		pBTR->close();
		pBTR = nullptr;

		AcDbPlotSettingsValidator *pPlotSettingsValidator = acdbHostApplicationServices()->plotSettingsValidator();
		if (pPlotSettingsValidator == nullptr) return false;

		pPlotSettingsValidator->refreshLists(pPlotSettings);	// Refresh the layout lists in order to use it
		es = pPlotSettingsValidator->setPlotCfgName(pPlotSettings, LR"(User 1)");	//设置图纸大小（A4）
		es = pPlotSettingsValidator->setPlotWindowArea(pPlotSettings, x0,y0,x1,y1); //设置打印范围
		es = pPlotSettingsValidator->setPlotOrigin(pPlotSettings, x0, y0);	//设置origin
		es = pPlotSettingsValidator->setPlotCentered(pPlotSettings, true);	//是否居中打印
		es = pPlotSettingsValidator->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);	//打印类型
		//es = pPlotSettingsValidator->setStdScale(pPlotSettings, dbScale);	//比例
		es = pPlotSettingsValidator->setStdScaleType(pLayout, AcDbPlotSettings::StdScaleType::kScaleToFit);//比例
																										   // we have to close the layout here because the last parameter of
																										   // findLayoutNamed is true, leave layout open
		es = PlotEngine->beginPlot(nullptr, nullptr);

		AcPlPlotInfo plotInfo;
		plotInfo.setLayout(idLayout);
		plotInfo.setOverrideSettings(pPlotSettings);

		AcPlPlotConfig *pPlotConfig = nullptr;
		es = acplPlotConfigManagerPtr()->setCurrentConfig(pPlotConfig, LR"(@AutoCAD PDF(High Quality Print).pc3)");
		pPlotConfig->setPlotToFile(true);
		pPlotConfig->refreshMediaNameList();
		plotInfo.setDeviceOverride(pPlotConfig);

		//开始打印
		AcPlPlotInfoValidator plotInfoValidator;
		AcPlPlotPageInfo pageInfo;
		plotInfoValidator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
		plotInfoValidator.validate(plotInfo);
		es = PlotEngine->beginDocument(plotInfo, acDocManager->curDocument()->fileName(),nullptr, 1, true, strFileName.c_str());
		es = PlotEngine->beginPage(pageInfo, plotInfo, true);
		es = PlotEngine->beginGenerateGraphics();
		PlotEngine->endGenerateGraphics();
		PlotEngine->endPage();
		PlotEngine->endDocument();
		PlotEngine->endPlot();

		return true;
	}

	bool setPlotArea(AcDbDatabase*$DB, const AcDbObjectId & varHBKID) {
		AcGePoint3d varBottomLeft;
		AcGePoint3d varTopRight;
		/*获得打印文件名称*/

		/*获得打印区域*/
		{
			sstd::ArxClosePointer< AcDbBlockReference > varR;
			if (eOk != acdbOpenObject(varR.pointer(), varHBKID)) {
				svthrow(LR"(can not open  AcDbBlockReference)");
				
			}
		}
	}

	void fitTextArea(AcDbDatabase*$DB,
		const AcDbObjectId & varHBKID,
		const AcDbObjectId & varBTLID) {

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
			fitTextArea(DB, varHBKIDS[0], varBTLIDS[0]);
		}

		/*设置打印区域*/
		if (varHBKIDS.size() == 1) {
			setPlotArea(DB, varHBKIDS[0]);
		}

	}
	catch (...) {}

}/*namespace sstd*/





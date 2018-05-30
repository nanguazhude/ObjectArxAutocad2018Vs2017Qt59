#include "EPrint.hpp"
#include <acedCmdNF.h>
#include <string>
#include <memory>
#include <set>
#include <optional>
#include <string_view>
#include <filesystem>

namespace sstd {
	extern void WCS2DCS(const double * i, double * o);
}

using namespace std::string_view_literals;
extern AcPlPlotProgressDialog * _sstd_acplCreatePlotProgressDialog();
/** This method returns
true when this database is using color-dependent plot styles, and
false for named plot styles.
See the AutoCAD User's Guide for more information
on the two types of plot styles.  **/
thread_local bool globalPlostStyle;

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
				varBR->setLayer(LR"(尺寸线)");

			}

			if (varBID.empty()) { return{}; }

			for (const auto & varJ : varEIDS_) {
				varEBID.insert(varJ);
			}

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

			//		if (varJustReturnOneID) {
			//			return { static_cast<std::size_t>(1) , *varJustReturnOneID };
			//		}
		}

		if (varBID.empty()) { return {}; }
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

	bool _setPlotArea(
		const double x0, const double y0,
		const double x1, const double y1,
		const wstring & strFileName) {

		bool varFlageK;
		{
			const auto varDx = std::abs(x0 - x1);
			const auto varDy = std::abs(y0 - y1);

			if (varDy < 0.00001) { svthrow(LR"(dy is too small)"sv); }
			if (varDx < 0.00001) { svthrow(LR"(dx is too small)"sv); }

			varFlageK = varDy > varDx;
		}

		acutPrintf(LR"(待打印文件:)");
		acutPrintf(strFileName.c_str());
		acutPrintf(LR"(
)");
		/*https://forums.autodesk.com/t5/objectarx/problem-of-plotting/m-p/6803724 */
		/*https://www.cnblogs.com/c-gis/articles/2214470.html */
		Acad::ErrorStatus es;

		class Locker {
		public:
			AcPlPlotEngine * PlotEngine = nullptr;
			AcDbBlockTableRecord *pBTR = nullptr;
			//AcDbLayout *pLayout = nullptr;
			AcPlPlotProgressDialog * pProgress = nullptr;
			~Locker() {
				if (PlotEngine)PlotEngine->destroy();
				if (pBTR) { pBTR->close(); }
				//if (pLayout) { pLayout->close(); }
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
		AcDbLayout *  pLayout = nullptr;
		AcDbObjectId idLayout = pBTR->getLayoutId();
		{   /*获得已经存在的打印样式...*/
			es = acdbOpenObject(pLayout, idLayout, AcDb::kForWrite);
			if (es != Acad::eOk) { return false; }
		}

		{
			class PLayoutLock {
				AcDbLayout * d;
			public:
				PLayoutLock(AcDbLayout *a) :d(a) {}
				~PLayoutLock() { d->close(); }
			}PlayoutLock(pLayout);

			//AcDbPlotSettingsValidator
			AcDbPlotSettingsValidator * pPlotSettingsValidator = acdbHostApplicationServices()->plotSettingsValidator();
			if (pPlotSettingsValidator == nullptr) { return false; }
			//plot dialog box
			//pPlotSettingsValidator->refreshLists(pLayout);	// Refresh the layout lists in order to use it
			//
			//pPlotSettingsValidator->setPlotViewName(pLayout, LR"(my_plot_view)");
			//pPlotSettingsValidator->setPlotViewName(pLayout,LR"()");
			//@AutoCAD PDF (High Quality Print).pc3
			es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF(High Quality Print).pc3)");	//设置打印设备
			if (es != eOk) {
				es = pPlotSettingsValidator->setPlotCfgName(pLayout, LR"(@AutoCAD PDF (High Quality Print).pc3)");	//设置打印设备
				if (es != eOk) {
					svthrow(LR"(无法找到:@AutoCAD PDF (High Quality Print).pc3)"sv);
				}
			}

			pPlotSettingsValidator->refreshLists(pLayout);	// Refresh the layout lists in order to use it
			{
				AcArray<const ACHAR *>  mediaList;
				pPlotSettingsValidator->canonicalMediaNameList(pLayout, mediaList);
				if (mediaList.length() < 1) { svthrow(LR"(can not find canonicalMediaNameList)"); }
				es = pPlotSettingsValidator->setCanonicalMediaName(pLayout, mediaList[0]);//设置图纸尺寸(A4?)
			}

			//es = pPlotSettingsValidator->setCanonicalMediaName(pLayout, LR"(User 1)");//设置图纸尺寸(A4?)
			{
				if (globalPlostStyle) {/*color*/
					es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout, LR"(monochrome.ctb)");//设置打印样式表
				}
				else {/*not color*/
					es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout, LR"(monochrome.stb)");//设置打印样式表
				}
			}
			//es = pPlotSettingsValidator->setCurrentStyleSheet(pLayout,_T("JSTRI.ctb"));//设置打印样式表
			//https://forums.autodesk.com/t5/net/setplotwindowarea-got-an-offset/m-p/6341629 /**/
			es = pPlotSettingsValidator->setPlotWindowArea(pLayout, x0, y0, x1, y1); //设置打印范围
			es = pPlotSettingsValidator->setPlotOrigin(pLayout, x0, y0);	//设置origin
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
			// apply to layout		 
			//pPlotSettingsValidator->setPlotViewName()
			// we have to close the layout here because the last parameter of
			pBTR->close();
			pBTR = nullptr;
		}

		const struct PlotEngineLock {
			AcPlPlotEngine * & d;
			PlotEngineLock(AcPlPlotEngine *&a, AcPlPlotProgressDialog *  p, Acad::ErrorStatus&es) :d(a) {
				es = d->beginPlot(p, nullptr);
				if (es != eOk) { svthrow(LR"(can not beginPlot)"sv); }
			}
			~PlotEngineLock() { d->endPlot(); }
		}varPlotEngineLock(PlotEngine, pProgress, es);																								   // findLayoutNamed is true, leave layout open


		{   /*获得已经存在的打印样式...*/
			es = acdbOpenObject(pLayout, idLayout, AcDb::kForRead);
			if (es != Acad::eOk) { return false; }
		}

		class PLayoutLock {
			AcDbLayout * d;
		public:
			PLayoutLock(AcDbLayout *a) :d(a) {}
			~PLayoutLock() { d->close(); }
		}PlayoutLock(pLayout);

		AcPlPlotInfo plotInfo;
		plotInfo.setLayout(idLayout);
		plotInfo.setOverrideSettings(pLayout)/*pLayout must be open by read...*/;

		if constexpr(false) {
			AcPlPlotConfig * pPlotConfig = nullptr;
			es = acplPlotConfigManagerPtr()->getCurrentConfig(pPlotConfig);/*, LR"(@AutoCAD PDF(High Quality Print).pc3)"*/
			if (es != eOk) {
				svthrow(LR"(can not getCurrentConfig)"sv);
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

	static inline std::string next_tmp_count() {
		constexpr const static unsigned short varSize = 8;
		static char varData[varSize];
		constexpr const static char varBC = 'a';
		constexpr const static char varBE = 'z';
		static bool varIsRun = []() {
			for (auto & i : varData) { i = varBC; }
			varData[0] = varBE;
			return (std::rand() & 1);
		}();
		auto varBegin = std::rbegin(varData);
		const auto varEnd = std::rend(varData);
		for (; varBegin != varEnd; ++varBegin) {
			auto & i = *varBegin;
			if (i==varBE/*the last ? */) {
				i = varBC;
				continue;
			}
			++i;
			break;
		}
		return std::string{ varData,varSize }+'/';
		(void)varIsRun;
	}

	static inline void show_pdf(std::wstring  strFileName) {
		QtApplication varQtApp;
		/******************************************************/
		static QTemporaryDir varDir{ QDir::temp().absoluteFilePath(QStringLiteral("autocad_pdf_tmp_show")) };
		const std::filesystem::path varSource = strFileName;
		const auto varTargetPathQ = varDir.filePath(QString::fromUtf8((next_tmp_count() + varSource.filename().u8string()).c_str()));
		strFileName = varTargetPathQ.toStdWString();
		const std::filesystem::path varTarget = strFileName;
		try {
			std::filesystem::create_directories(varTarget.parent_path());
			std::filesystem::copy(varSource, varTarget, std::filesystem::copy_options::overwrite_existing);
		}
		catch (...) {
			return;
		}
		/******************************************************/
		const auto varStartCode = QStringLiteral("mupdf ") +
			(QChar('\"') + QString::fromStdWString(strFileName) + QChar('\"'));
		if (false == QProcess::startDetached(varStartCode)) {
			QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdWString(strFileName)));
		}
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
			const bool varHasFinal = varFileInfo.fileName()
				.indexOf(QStringLiteral("final"), 0, Qt::CaseInsensitive) >= 0;
			QDir varDirTmp{ varFilePath };
			const auto varBaseName = [&varFileInfo]() ->QString {
				auto varBaseName = varFileInfo.completeBaseName();
				auto varTmp = varBaseName.split(QChar('.'), QString::SkipEmptyParts);
				if (varTmp.isEmpty()) { svthrow(LR"(logical error!)"); }
				if (varTmp.size() == 1) { return *varTmp.cbegin(); }
				if ((varTmp.crbegin()->compare(QStringLiteral("final"), Qt::CaseInsensitive) == 0) ||
					(varTmp.crbegin()->compare(QStringLiteral("unchecked"), Qt::CaseInsensitive) == 0)) {
					varTmp.pop_back();
				}
				varBaseName = *varTmp.cbegin();
				varTmp.pop_front();
				for (const auto & varI : std::as_const(varTmp)) {
					varBaseName += QChar('.');
					varBaseName += varI;
				}
				return std::move(varBaseName);
			}();
			const auto varFinalName = varDirTmp.absoluteFilePath(varBaseName
				+ QString::fromUtf8(u8R"(.Final.pdf)"));
			const auto varUncheckedName = varDirTmp.absoluteFilePath(varBaseName
				+ QString::fromUtf8(u8R"(.unchecked.pdf)"));
			const auto & varPlotFileName = varHasFinal ? varFinalName : varUncheckedName;

			if constexpr(false) {
				const auto varPlotFileNameBack = varPlotFileName + QStringLiteral(".back");
				varFileName.assign(varPlotFileName.toStdWString());
				if (QFile::exists(varPlotFileNameBack)) {
					if (false == QFile::remove(varPlotFileNameBack)) {
						svthrow(LR"(无法删除已经存在的文件!)"sv);
					}
				}
				if (QFile::exists(varPlotFileName)) {
					QFile::copy(varPlotFileName, varPlotFileNameBack);
					if (false == QFile::remove(varPlotFileName)) {
						svthrow(LR"(无法删除已经存在的文件!)"sv);
					}
				}
			}	/*if constexpr */	else {
				varFileName.assign(varPlotFileName.toStdWString());
				if (QFile::exists(varFinalName)) {
					if (false == QFile::remove(varFinalName)) {
						svthrow(LR"(无法删除已经存在的文件!)"sv);
					}
				}
				if (QFile::exists(varUncheckedName)) {
					if (false == QFile::remove(varUncheckedName)) {
						svthrow(LR"(无法删除已经存在的文件!)"sv);
					}
				}
			}

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

		{

			/* zoom */
			class Lock {
			public:
				ads_name ss = {};
				Lock() { construct(); }
				void construct() { acedSSAdd(nullptr, nullptr, ss); }
				void destory() { acedSSFree(ss); ss[0] = 0; ss[1] = 0; }
				~Lock() { destory(); }
			} varLock;

			{
				ads_name s;
				acdbGetAdsName(s, varHBKID);
				acedSSAdd(s, varLock.ss, varLock.ss);
			}

			acedCommandS(
				RTSTR, LR"(ZOOM)",
				RTSTR, L"O",
				RTPICKS, varLock.ss,
				RTSTR, L"",
				RTNONE);
		}

		class PlotLock {
			//BACKGROUNDPLOT
		private:
			struct resbuf $BACKGROUNDPLOT;
		public:
			PlotLock() {
				acedGetVar(LR"(BACKGROUNDPLOT)", &$BACKGROUNDPLOT);
				static class SetVar {
					struct resbuf $BACKGROUNDPLOT;
				public:
					SetVar() {
						$BACKGROUNDPLOT.rbnext = nullptr;
						$BACKGROUNDPLOT.restype = RTSHORT;
						$BACKGROUNDPLOT.resval.rint = 0;
					}
					void set() {
						acedSetVar(LR"(BACKGROUNDPLOT)", &$BACKGROUNDPLOT);
					}
				}setVar;
				setVar.set();
			}
			~PlotLock() {
				acedSetVar(LR"(BACKGROUNDPLOT)", &$BACKGROUNDPLOT);
			}
		}varPlotLock;

		WCS2DCS(&varBottomLeft.x, &varBottomLeft.x);
		WCS2DCS(&varTopRight.x, &varTopRight.x);

		if (_setPlotArea(
			varBottomLeft.x, varBottomLeft.y,
			varTopRight.x, varTopRight.y,
			varFileName)) {
			show_pdf(varFileName);
			return true;
		}
		return false;

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
		varBTL->setPosition({ varX, varTopY - 0.5 ,0 });
	}

	void EPrint::main() try {
		auto DB = acdbHostApplicationServices()->workingDatabase();
		globalPlostStyle = DB->plotStyleMode();
		//print_all_used_block_name(DB);
		std::vector<AcDbObjectId> varBTLIDS;
		std::vector<AcDbObjectId> varHBKIDS;
		try {
			varBTLIDS = setBlockDrawOrder<false>(DB, LR"(@标题栏文字(1))"sv);
		}
		catch (...) {
			acutPrintf(LR"(无法找到@标题栏文字(1)
)");
		}

		try {
			varHBKIDS = setBlockDrawOrder<true>(DB, LR"(横边框2(G3000))"sv);
		}
		catch (...) {
			acutPrintf(LR"(无法找到@横边框2(G3000)
)");
		}

		acutPrintf(LR"(
@标题栏文字(1) : %d
)", varBTLIDS.size());
		acutPrintf(LR"(
@横边框2(G3000) : %d
)", varHBKIDS.size());

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

///



#include <regex>
#include <optional>
#include <dbAnnotativeObjectPE.h>
#include "UpdateDimStyle.hpp"

namespace sstd {

	UpdateDimStyle::UpdateDimStyle() {

	}

	void UpdateDimStyle::load() {
		arx_add_main_command<UpdateDimStyle>();
	}

	namespace {

		template<typename T, typename U>
		inline void _copy(T * argC, const U * argR) {
			argC->setDimadec(argR->dimadec());
			argC->setDimalt(argR->dimalt());
			argC->setDimaltd(argR->dimaltd());
			argC->setDimaltf(argR->dimaltf());
			argC->setDimaltmzf(argR->dimaltmzf());
			argC->setDimaltmzs(argR->dimaltmzs());
			argC->setDimaltrnd(argR->dimaltrnd());
			argC->setDimalttd(argR->dimalttd());
			argC->setDimalttz(argR->dimalttz());
			argC->setDimaltu(argR->dimaltu());
			argC->setDimaltz(argR->dimaltz());
			argC->setDimapost(argR->dimapost());
			argC->setDimarcsym(argR->dimarcsym());
			argC->setDimasz(argR->dimasz());
			argC->setDimatfit(argR->dimatfit());
			argC->setDimaunit(argR->dimaunit());
			argC->setDimazin(argR->dimazin());
			argC->setDimblk(argR->dimblk());
			argC->setDimblk1(argR->dimblk1());
			argC->setDimblk2(argR->dimblk2());
			argC->setDimcen(argR->dimcen());
			argC->setDimclrd(argR->dimclrd());
			argC->setDimclre(argR->dimclre());
			argC->setDimclrt(argR->dimclrt());
			argC->setDimdec(argR->dimdec());
			argC->setDimdle(argR->dimdle());
			argC->setDimdli(argR->dimdli());
			argC->setDimdsep(argR->dimdsep());
			argC->setDimexe(argR->dimexe());
			argC->setDimexo(argR->dimexo());
			argC->setDimfrac(argR->dimfrac());
			argC->setDimgap(argR->dimgap());
			argC->setDimjogang(argR->dimjogang());
			argC->setDimjust(argR->dimjust());
			argC->setDimldrblk(argR->dimldrblk());
			argC->setDimlfac(argR->dimlfac());
			argC->setDimlim(argR->dimlim());
			argC->setDimltex1(argR->dimltex1());
			argC->setDimltex2(argR->dimltex2());
			argC->setDimltype(argR->dimltype());
			argC->setDimlunit(argR->dimlunit());
			argC->setDimlwd(argR->dimlwd());
			argC->setDimlwe(argR->dimlwe());
			argC->setDimmzf(argR->dimmzf());
			argC->setDimmzs(argR->dimmzs());
			argC->setDimpost(argR->dimpost());
			argC->setDimrnd(argR->dimrnd());
			argC->setDimsah(argR->dimsah());
			argC->setDimscale(argR->dimscale());
			argC->setDimsd1(argR->dimsd1());
			argC->setDimsd2(argR->dimsd2());
			argC->setDimse1(argR->dimse1());
			argC->setDimse2(argR->dimse2());
			argC->setDimsoxd(argR->dimsoxd());
			argC->setDimtad(argR->dimtad());
			argC->setDimtdec(argR->dimtdec());
			argC->setDimtfac(argR->dimtfac());
			argC->setDimtfill(argR->dimtfill());
			argC->setDimtfillclr(argR->dimtfillclr());
			//argC->setDimtih(argR->dimtih());
			argC->setDimtix(argR->dimtix());
			argC->setDimtm(argR->dimtm());
			argC->setDimtmove(argR->dimtmove());
			argC->setDimtofl(argR->dimtofl());
			//argC->setDimtoh(argR->dimtoh());
			argC->setDimtol(argR->dimtol());
			argC->setDimtolj(argR->dimtolj());
			argC->setDimtp(argR->dimtp());
			argC->setDimtsz(argR->dimtsz());
			argC->setDimtvp(argR->dimtvp());
			argC->setDimtxsty(argR->dimtxsty());
			argC->setDimtxt(argR->dimtxt());
			argC->setDimtxtdirection(argR->dimtxtdirection());
			argC->setDimtzin(argR->dimtzin());
			argC->setDimupt(argR->dimupt());
			argC->setDimzin(argR->dimzin());
			argC->setDimfxlen(argR->dimfxlen());
		}

		class ThisState {
		public:
			std::map<std::wstring, AcDbObjectId, std::less<>> $AllTextStyle;
			std::map<std::wstring, AcDbObjectId, std::less<>> $AllBlockStyle;

			using RT = std::optional<AcDbObjectId>;

			inline RT getTextStyle(const std::wstring_view &arg) const {
				auto varPos = $AllTextStyle.find(arg);
				if (varPos != $AllTextStyle.end()) {
					return  varPos->second;
				}
				return {};
			}

			inline RT getBlock(const std::wstring_view &arg) const {
				auto varPos = $AllBlockStyle.find(arg);
				if (varPos != $AllBlockStyle.end()) {
					return  varPos->second;
				}
				return {};
			}

		};

		using StyleTable = AcDbDimStyleTable;
		using StyleTableRecord = AcDbDimStyleTableRecord;
		using StyleTableIterator = AcDbDimStyleTableIterator;
#define simple_code_args const std::wstring_view & argNM,StyleTable * argTST,StyleTableRecord * argR
		using ApplyLayerType = void(*)(simple_code_args,
			StyleTableRecord *, StyleTableRecord *,
			StyleTableRecord *, StyleTableRecord *,
			ThisState*);
		using ApplyMaps = std::map<std::wstring_view, std::pair<ApplyLayerType, bool> >;

		inline auto setAnnotative(
			AcDbObject* argObject,
			bool argIsAnnotative) {
			AcDbAnnotativeObjectPE * varAnnoPE = ACRX_PE_PTR(
				argObject,
				AcDbAnnotativeObjectPE);
			if (varAnnoPE == nullptr) {
				return Acad::eNullPtr;
			}

			if (varAnnoPE->annotative(argObject) == argIsAnnotative) {
				return Acad::eOk;
			}

			return varAnnoPE->setAnnotative(argObject,
				argIsAnnotative);
		}

		inline void/*线性标注*/_linear_child(simple_code_args, StyleTableRecord *argC) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($0)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argC == nullptr) {
				argC = new StyleTableRecord;
				varLocalR = argC;
				argC->setName(varName.data());
				argTST->add(argC);
			}

			_copy(argC, argR);

			/*与尺寸线对齐*/
			argC->setDimtoh(false);
			argC->setDimtih(false);
			setAnnotative(argC, true)/*注释性*/;
		}
		inline void/*角度标注*/_angular_child(simple_code_args, StyleTableRecord *argC) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($2)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argC == nullptr) {
				argC = new StyleTableRecord;
				varLocalR = argC;
				argC->setName(varName.data());
				argTST->add(argC);
			}

			_copy(argC, argR);

			/*强制水平*/
			argC->setDimtih(true);
			argC->setDimtoh(true);
			setAnnotative(argC, true)/*注释性*/;
		}
		inline void/*直径标注*/_diameter_child(simple_code_args, StyleTableRecord *argC) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($3)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argC == nullptr) {
				argC = new StyleTableRecord;
				varLocalR = argC;
				argC->setName(varName.data());
				argTST->add(argC);
			}

			_copy(argC, argR);

			/*文在在尺寸界限内时对齐尺寸线,在尺寸线外时水平对齐*/
			argC->setDimtih(false);
			argC->setDimtoh(true);
			setAnnotative(argC, true)/*注释性*/;
		}

		inline void/*半径标注*/_radius_child(simple_code_args, StyleTableRecord *argC) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($4)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argC == nullptr) {
				argC = new StyleTableRecord;
				varLocalR = argC;
				argC->setName(varName.data());
				argTST->add(argC);
			}

			_copy(argC, argR);

			/*文在在尺寸界限内时对齐尺寸线,在尺寸线外时水平对齐*/
			argC->setDimtih(false);
			argC->setDimtoh(true);
			//argR->setDimtofl(false)/*不绘制尺寸界限内的尺寸线*/;
			setAnnotative(argC, true)/*注释性*/;
		}

		inline AcCmColor operator""_ac(unsigned long long arg) {
			AcCmColor varAns;
			varAns.setColorIndex(static_cast<std::uint16_t>(arg));
			return std::move(varAns);
		}

		inline ApplyMaps _p_createFunctions() {
			ApplyMaps varAns;
/************************************/
#include "appendDimStyle/ISO-25.hpp"
#include "appendDimStyle/Standard.hpp"
#include "appendDimStyle/Annotative.hpp"
/************************************/
			varAns.emplace(LR"(默认公差)", ApplyMaps::value_type::second_type{
				[](simple_code_args,
				StyleTableRecord * arg0,StyleTableRecord * arg2,
				StyleTableRecord * arg3,StyleTableRecord * arg4,
				ThisState * argGl) {
				/*https://knowledge.autodesk.com/zh-hans/support/autocad/troubleshooting/caas/sfdcarticles/sfdcarticles/CHS/Creating-a-new-dimension-style-on-the-command-line.html*/
/******************************************************************/
				sstd::ArxClosePointer<StyleTableRecord> varLocalR;
				if (argR == nullptr) {
					argR = new StyleTableRecord;
					varLocalR = argR;
					argR->setName(argNM.data());
					argTST->add(argR);
				}

				/*线*******************************/
				{
					argR->setDimclrd(102_ac)/*尺寸线颜色*/;
					argR->setDimclre(102_ac)/*尺寸界限颜色*/;
					argR->setDimdli(7.0)/*基线间距*/;
					argR->setDimexe(2.0)/*尺寸界限超出尺寸线距离*/;
					argR->setDimexo(0.0);
					argR->setDimsd2(false);
				}
				/*符号和箭头***********************/
				{
					constexpr const auto varRowInit = LR"(_MY_ROW)"sv;
					const auto varRowType = argGl->getBlock(varRowInit);
					if (varRowType) {
						argR->setDimblk(*varRowType);
						argR->setDimblk1(*varRowType)/*第一个箭头*/;
						argR->setDimblk2(*varRowType)/*第二个箭头*/;
					}
					else {
						argR->setDimblk(LR"()");
						argR->setDimblk1(LR"()")/*第一个箭头*/;
						argR->setDimblk2(LR"()")/*第二个箭头*/;
					}
					argR->setDimasz(4.6)/*箭头长度*/;
					argR->setDimcen(0.0)/*圆心标记*/;
				}
				/*文字****************************/
				{
					const auto varTextType = argGl->getTextStyle(LR"(@Standard)");
					if (varTextType) { argR->setDimtxsty(*varTextType); }
					argR->setDimclrt(111_ac)/*文字颜色*/;
					argR->setDimgap(1.25)/*尺寸线和文字的间距*/;
					argR->setDimtfill(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-4E38E29F-DE85-4791-A2E7-4DC22842B1B4-htm.html*/;
					argR->setDimtad(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-60D1241D-CEA7-4493-BD6A-4EF433F3C946-htm.html*/;
					argR->setDimjust(0)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-C67348A9-2260-4135-A7FF-FE0B45211CB0-htm.html*/;
					/*文在在尺寸界限内时对齐尺寸线,在尺寸线外时水平对齐*/
					argR->setDimtih(false);
					argR->setDimtoh(true);
				}
				/*调整****************************/
				{
					setAnnotative(argR, true)/*注释性*/;
					argR->setDimatfit(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-2F8FA7E7-C79F-40EA-A416-C4B02D09F310-htm.html*/;
					argR->setDimtofl(true)/*强制绘制尺寸线*/;
				}
				/*主单位**************************/
				{
					argR->setDimlunit(2)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-2ECDF7CF-6EEA-4174-B50C-8630D5002C20-htm.html*/;
					argR->setDimdec(2)/*小数0.00*/;
					argR->setDimdsep(wchar_t('.'))/*小数分隔符*/;
					argR->setDimaunit(0)/*十进制度数*/;
					argR->setDimadec(2)/*0.00*/;
					argR->setDimzin(8)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-A1860981-FE1C-4947-927B-7CD6B8CEF8EE-htm.html*/;
					argR->setDimazin(2)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-69E9BFD3-06A4-468D-88F0-7C9741407194-htm.html*/;
				}
				/*换算单位*/
				{
				}
				/*公差*/
				{
					argR->setDimtol(false)/*不显示公差*/;
					argR->setDimtdec(2)/*小数0.00*/;
					argR->setDimtfac(1)/*公差高度比*/;
					argR->setDimtp(0.4)/*公差最大值*/;
					argR->setDimtm(0.4)/*公差最小值*/;
					argR->setDimtzin(8)/*公差消零*/;
					argR->setDimalttz(8)/*公差消零*/;
					argR->setDimtolj(1)/*公差显示位置*/;
				}

				/******************************************************************/
				{
					_linear_child(argNM, argTST, argR, arg0);
					_angular_child(argNM, argTST, argR, arg2);
					_diameter_child(argNM, argTST, argR, arg3);
					_radius_child(argNM, argTST, argR, arg4);
				}
							},false });
/*对称公差******************************************/
#include "appendDimStyle/对称公差.hpp"
#include "appendDimStyle/对称公差_0P12.hpp"
#include "appendDimStyle/对称公差_0P15.hpp"
/*极限公差******************************************/
#include "appendDimStyle/极限公差.hpp"
#include "appendDimStyle/极限公差_0P1.hpp"
#include "appendDimStyle/极限公差_0P2.hpp"
#include "appendDimStyle/极限公差_0P3.hpp"
#include "appendDimStyle/极限公差_0P8.hpp"
/*一半公差******************************************/
#include "appendDimStyle/一半_默认公差.hpp"
/*加框公差******************************************/
#include "appendDimStyle/加框_默认公差.hpp"
/***************************************************/

			return std::move(varAns);
		}

		inline std::unique_ptr<ThisState> _p_get_state(AcDbDatabase * argDB) {
			std::unique_ptr<ThisState> varAns{ new ThisState };
			
			/*获得所有文字样式的ID*/
			{
				sstd::ArxClosePointer< AcDbTextStyleTable > varT;
				std::unique_ptr< AcDbTextStyleTableIterator > varI;
				argDB->getTextStyleTable(varT);
				{
					AcDbTextStyleTableIterator * varTmp;
					varT->newIterator(varTmp);
					varI.reset(varTmp);
				}
				for (; !varI->done(); varI->step()) {
					const wchar_t * varTN = nullptr;
					sstd::ArxClosePointer< AcDbTextStyleTableRecord > varR;
					varI->getRecord(varR);
					varR->getName(varTN);
					varAns->$AllTextStyle.emplace(varTN, varR->objectId());
				}
			}

			/*获得所有块的ID*/
			{
				sstd::ArxClosePointer< AcDbBlockTable > varT;
				std::unique_ptr< AcDbBlockTableIterator > varI;
				argDB->getBlockTable(varT);
				{
					AcDbBlockTableIterator * varTmp;
					varT->newIterator(varTmp);
					varI.reset(varTmp);
				}
				for (; !varI->done(); varI->step()) {
					const wchar_t * varTN = nullptr;
					sstd::ArxClosePointer< AcDbBlockTableRecord > varR;
					varI->getRecord(varR);
					varR->getName(varTN);
					varAns->$AllTextStyle.emplace(varTN, varR->objectId());
				}
			}

			return std::move(varAns);
		}

		inline void _p_update_dim_style(AcDbDatabase * argDB) {
			const static std::wregex varRJ(LR"(.*\$[0234])");
			sstd::ArxClosePointer< StyleTable > varDimStyleTable;
			if (Acad::eOk != argDB->getDimStyleTable(varDimStyleTable, AcDb::kForWrite)) {
				acutPrintf(LR"(获得文字样式失败
)"); return;
			}

			std::unique_ptr<ThisState> varThisState = _p_get_state(argDB);
			auto varFunctions = _p_createFunctions();
			const auto varNoPos = varFunctions.end();

			/*处理已经存在的文字样式*/
			class Item {
			public:
				sstd::ArxClosePointer<StyleTableRecord> r;
			};
			std::map< std::wstring, std::shared_ptr<Item> >
				varAllStyleTableRecord;
			{
				std::unique_ptr<StyleTableIterator> varIt;
				{
					StyleTableIterator * var = nullptr;
					if (Acad::eOk != varDimStyleTable->newIterator(var)) {
						acutPrintf(LR"(迭代文字样式失败
)"); return;
					}
					varIt.reset(var);
				}

				for (; !varIt->done(); varIt->step()) {
					auto varDimStyleTableRecord = std::make_shared<Item>();
					if (Acad::eOk != varIt->getRecord(varDimStyleTableRecord->r, AcDb::kForWrite)) {
						continue;
					}
					const wchar_t * varDimStyleName = nullptr;
					if (Acad::eOk != varDimStyleTableRecord->r->getName(varDimStyleName)) {
						continue;
					}
					varAllStyleTableRecord.emplace(varDimStyleName,
						std::move(varDimStyleTableRecord));

				}

			}

			{
				const auto varNoPosx = varAllStyleTableRecord.end();
				auto get_0 = [&varAllStyleTableRecord, &varNoPosx](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($0)");
					if (varPos == varNoPosx) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_2 = [&varAllStyleTableRecord, &varNoPosx](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($2)");
					if (varPos == varNoPosx) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_3 = [&varAllStyleTableRecord, &varNoPosx](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($3)");
					if (varPos == varNoPosx) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_4 = [&varAllStyleTableRecord, &varNoPosx](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($4)");
					if (varPos == varNoPosx) { return nullptr; }
					return varPos->second->r.pointer();
				};

				for (const auto & varII : varAllStyleTableRecord) {
					const auto & varDimStyleNameW = varII.first;
					if (std::regex_match(varDimStyleNameW, varRJ)) {
						continue;
					}

					auto varPos = varFunctions.find(varDimStyleNameW);
					if (varPos == varNoPos) { continue; }
					varPos->second.second = true;
					varPos->second.first(varDimStyleNameW,
						varDimStyleTable,
						varII.second->r,
						get_0(varDimStyleNameW),
						get_2(varDimStyleNameW),
						get_3(varDimStyleNameW),
						get_4(varDimStyleNameW),
						varThisState.get());
				}

			}

			/*close opened record*/
			varAllStyleTableRecord.clear();

			/*新增文字样式*/
			{
				for (auto & varI : varFunctions) {
					if (varI.second.second) { continue; }
					varI.second.second = true;
					varI.second.first(varI.first,
						varDimStyleTable,
						nullptr,
						nullptr,
						nullptr,
						nullptr,
						nullptr,
						varThisState.get());
				}
			}

		}

	}/*namespace*/

	void UpdateDimStyle::main() {
		_p_update_dim_style(acdbHostApplicationServices()->workingDatabase());
	}

	extern void loadUpdateDimStyle() { UpdateDimStyle::load(); }

}/*namespace sstd*/


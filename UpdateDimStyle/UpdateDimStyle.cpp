#include <regex>
#include "UpdateDimStyle.hpp"

namespace sstd {

	UpdateDimStyle::UpdateDimStyle() {

	}

	void UpdateDimStyle::load() {
		arx_add_main_command<UpdateDimStyle>();
	}

	namespace {
		using StyleTable = AcDbDimStyleTable;
		using StyleTableRecord = AcDbDimStyleTableRecord;
		using StyleTableIterator = AcDbDimStyleTableIterator;
#define simple_code_args const std::wstring_view & argNM,StyleTable * argTST,StyleTableRecord * argR
		using ApplyLayerType = void(*)(simple_code_args, StyleTableRecord *, StyleTableRecord *, StyleTableRecord *, StyleTableRecord *);
		using ApplyMaps = std::map<std::wstring_view, std::pair<ApplyLayerType, bool> >;
		
		inline void/*线型标注*/_linear_child(simple_code_args) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($0)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argR == nullptr) {
				argR = new StyleTableRecord;
				varLocalR = argR;
				argR->setName(varName.data());
				argTST->add(argR);
			}

		}
		inline void/*角度标注*/_angular_child(simple_code_args) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($2)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argR == nullptr) {
				argR = new StyleTableRecord;
				varLocalR = argR;
				argR->setName(varName.data());
				argTST->add(argR);
			}

		}
		inline void/*直径标注*/_diameter_child(simple_code_args) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($3)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argR == nullptr) {
				argR = new StyleTableRecord;
				varLocalR = argR;
				argR->setName(varName.data());
				argTST->add(argR);
			}

		}
		inline void/*半径标注*/_radius_child(simple_code_args) {
			std::wstring varName{ argNM.begin(),argNM.end() };
			varName += LR"($4)"sv;

			sstd::ArxClosePointer<StyleTableRecord> varLocalR;
			if (argR == nullptr) {
				argR = new StyleTableRecord;
				varLocalR = argR;
				argR->setName(varName.data());
				argTST->add(argR);
			}

		}
		
		inline ApplyMaps _p_createFunctions() {
			ApplyMaps varAns;
			varAns.emplace(LR"(MTest)", ApplyMaps::value_type::second_type{ 
				[](simple_code_args,
				StyleTableRecord * arg0,
				StyleTableRecord * arg2,
				StyleTableRecord * arg3,
				StyleTableRecord * arg4) {

				sstd::ArxClosePointer<StyleTableRecord> varLocalR;
				if (argR == nullptr) {
					argR = new StyleTableRecord;
					varLocalR = argR;
					argR->setName(argNM.data());
					argTST->add(argR);
				}
				
				_linear_child(argNM, argTST,arg0);
				_angular_child(argNM, argTST,arg2);
				_diameter_child(argNM, argTST,arg3);
				_radius_child(argNM, argTST,arg4);
				
			},false });
			return std::move(varAns);
		}

		void _p_update_dim_style(AcDbDatabase * argDB) {

			sstd::ArxClosePointer< StyleTable > varDimStyleTable;
			if (Acad::eOk != argDB->getDimStyleTable(varDimStyleTable, AcDb::kForWrite)) {
				acutPrintf(LR"(获得文字样式失败
)"); return;
			}

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
					varAllStyleTableRecord.emplace(varDimStyleName,std::move(varDimStyleTableRecord));
					/*const std::wstring_view varDimStyleNameW(varDimStyleName);
					auto varPos = varFunctions.find(varDimStyleNameW);
					if (varPos == varNoPos) { continue; }
					varPos->second.second = true;
					varPos->second.first(varDimStyleNameW,
						varDimStyleTable,
						varDimStyleTableRecord,
						nullptr,
						nullptr,
						nullptr,
						nullptr);*/
				}

			}

			{
				const auto varNoPos = varAllStyleTableRecord.end();
				const static std::wregex varRJ(LR"($[0234])");
				auto get_0 = [&varAllStyleTableRecord,&varNoPos](const std::wstring &arg) 
				->StyleTableRecord*{
					auto varPos = varAllStyleTableRecord.find(arg+LR"($0)");
					if (varPos == varNoPos) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_2 = [&varAllStyleTableRecord, &varNoPos](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($2)");
					if (varPos == varNoPos) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_3 = [&varAllStyleTableRecord, &varNoPos](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($3)");
					if (varPos == varNoPos) { return nullptr; }
					return varPos->second->r.pointer();
				};
				auto get_4 = [&varAllStyleTableRecord, &varNoPos](const std::wstring &arg)
					->StyleTableRecord* {
					auto varPos = varAllStyleTableRecord.find(arg + LR"($4)");
					if (varPos == varNoPos) { return nullptr; }
					return varPos->second->r.pointer();
				};

				for (const auto & varII : varAllStyleTableRecord) {
					if (std::regex_match(varII.first, varRJ)) { 
						continue;
					}

				}

			}

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
						nullptr);
				}
			}

		}
		
	}/*namespace*/

	void UpdateDimStyle::main() {
		_p_update_dim_style(acdbHostApplicationServices()->workingDatabase());
	}

	extern void loadUpdateDimStyle() { UpdateDimStyle::load(); }

}/*namespace sstd*/


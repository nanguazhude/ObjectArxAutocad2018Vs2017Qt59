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
		using ApplyLayerType = void(*)(simple_code_args);
		using ApplyMaps = std::map<std::wstring_view, std::pair<ApplyLayerType, bool> >;
		inline ApplyMaps _p_createFunctions() {
			ApplyMaps varAns;
			varAns.emplace(LR"()", ApplyMaps::value_type::second_type{ 
				[](simple_code_args) {

				sstd::ArxClosePointer<StyleTableRecord> varLocalR;
				if (argR == nullptr) {
					argR = new StyleTableRecord;
					varLocalR = argR;
					argR->setName(argNM.data());
					argTST->add(argR);
				}
				
				
				
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
					sstd::ArxClosePointer<StyleTableRecord> varDimStyleTableRecord;
					if (Acad::eOk != varIt->getRecord(varDimStyleTableRecord, AcDb::kForWrite)) {
						continue;
					}
					const wchar_t * varDimStyleName = nullptr;
					if (Acad::eOk != varDimStyleTableRecord->getName(varDimStyleName)) {
						continue;
					}
					const std::wstring_view varDimStyleNameW(varDimStyleName);
					auto varPos = varFunctions.find(varDimStyleNameW);
					if (varPos == varNoPos) { continue; }
					varPos->second.second = true;
					varPos->second.first(varDimStyleNameW,
						varDimStyleTable,
						varDimStyleTableRecord);
				}

			}

			/*新增文字样式*/
			{
				for (auto & varI : varFunctions) {
					if (varI.second.second) { continue; }
					varI.second.second = true;
					varI.second.first(varI.first,
						varDimStyleTable,
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


#include <dbAnnotativeObjectPE.h>
#include <map>
#include "UpdateTextStyle.hpp"
 

namespace sstd {

	UpdateTextStyle::UpdateTextStyle() {

	}

	void UpdateTextStyle::load() {
		arx_add_main_command<UpdateTextStyle>();
	}

	namespace {
#define simple_code_args const std::wstring_view & argNM,AcDbTextStyleTable * argTST,AcDbTextStyleTableRecord * argR
		using ApplyLayerType = void(*)(simple_code_args);
		using ApplyMaps = std::map<std::wstring_view, std::pair<ApplyLayerType, bool> >;
		
		inline auto setAnnotative(
			AcDbObject* argObject,
			bool argIsAnnotative) {
			AcDbAnnotativeObjectPE * varAnnoPE = ACRX_PE_PTR(
				argObject,
				AcDbAnnotativeObjectPE);
			if ( varAnnoPE == nullptr ) {
				return Acad::eNullPtr;
			}

			if (varAnnoPE->annotative(argObject) == argIsAnnotative) {
				return Acad::eOk;
			}

			return varAnnoPE->setAnnotative(argObject,
				argIsAnnotative);
		}
		
		inline ApplyMaps _p_createFunctions() {
			ApplyMaps varAns;
			varAns.emplace(LR"(@Basic)"sv, ApplyMaps::value_type::second_type{
				[](simple_code_args) {
				sstd::ArxClosePointer<AcDbTextStyleTableRecord> varLocalR;
				if (argR == nullptr) {
					argR = new AcDbTextStyleTableRecord;
					varLocalR = argR;
					argR->setName(argNM.data());
					argTST->add(argR);
				}

				argR->setFlagBits(argR->flagBits()&0b11111001)/*清除标志位*/;
				argR->setIsShapeFile(false);
				argR->setFileName(LR"(ztxt.shx)")/*shx字体文件名*/;
				argR->setBigFontFileName(LR"(whtmtxt.shx)")/*shx大字体文件名*/;
				argR->setTextSize(5.7)/*文字高度*/;
				if (argR->priorSize()<5) {
					argR->setPriorSize(5.7)/*默认大小*/;
				} 
				argR->setXScale(1.0)/*宽度比*/;
				setAnnotative(argR,true)/*注释性*/;

			},false });
#include "appendStyle/@Standard.hpp"
#include "appendStyle/@YD标题栏文字.hpp"
#include "appendStyle/@块.hpp"
#include "appendStyle/@块机械文字.hpp"
#include "appendStyle/Annotative.hpp"
#include "appendStyle/Standard.hpp"
#include "appendStyle/宋体.hpp"
#include "appendStyle/技术要求.hpp"
#include "appendStyle/机械.hpp"
#include "appendStyle/标准.hpp"
#include "appendStyle/标注汉字.hpp"
#include "appendStyle/标题栏.hpp"
#include "appendStyle/界限ABCDEF.hpp"
			return std::move(varAns);
		}
		inline void _p_update_text_style(AcDbDatabase * argDB) {
			
			sstd::ArxClosePointer< AcDbTextStyleTable > varTextStyleTable;
			if (Acad::eOk != argDB->getTextStyleTable(varTextStyleTable, AcDb::kForWrite)) {
				acutPrintf(LR"(获得文字样式失败
)"); return;
			}

			auto varFunctions = _p_createFunctions();
			const auto varNoPos = varFunctions.end();

			/*处理已经存在的文字样式*/
			{
				std::unique_ptr<AcDbTextStyleTableIterator> varIt;
				{
					AcDbTextStyleTableIterator * var = nullptr;
					if (Acad::eOk != varTextStyleTable->newIterator(var)) {
						acutPrintf(LR"(迭代文字样式失败
)"); return;
					}
					varIt.reset(var);
				}

				for (; !varIt->done(); varIt->step()) {
					sstd::ArxClosePointer<AcDbTextStyleTableRecord> varTextStyleTableRecord;
					if (Acad::eOk != varIt->getRecord(varTextStyleTableRecord, AcDb::kForWrite)) {
						continue;
					}
					const wchar_t * varTextStyleName = nullptr;
					if (Acad::eOk != varTextStyleTableRecord->getName(varTextStyleName)) {
						continue;
					}
					const std::wstring_view varTextStyleNameW(varTextStyleName);
					auto varPos = varFunctions.find(varTextStyleNameW);
					if (varPos == varNoPos) { continue; }
					varPos->second.second = true;
					varPos->second.first(varTextStyleNameW,
						varTextStyleTable,
						varTextStyleTableRecord);
				}

			}

			/*新增文字样式*/
			{
				for (auto & varI : varFunctions) {
					if (varI.second.second) { continue; }
					varI.second.second = true;
					varI.second.first(varI.first,
						varTextStyleTable,
						nullptr);
				}
			}

		}
	}/*namespace*/

	void UpdateTextStyle::main() {
		_p_update_text_style(acdbHostApplicationServices()->workingDatabase());
	}

	extern void loadUpdateTextStyle() { UpdateTextStyle::load(); }

}/*namespace sstd*/

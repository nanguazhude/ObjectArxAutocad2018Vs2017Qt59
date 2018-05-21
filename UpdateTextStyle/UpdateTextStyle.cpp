#include <dbAnnotativeObjectPE.h>
#include <map>
#include "UpdateTextStyle.hpp"
 

namespace sstd {

	UpdateTextStyle::UpdateTextStyle() {

	}

	namespace {
		class UpdateTextStyle_1 {
		public:
			static void main() {
				UpdateTextStyle::main_do_not_change_text_height();
			}
			DEFINE_ARX_NAME(LR"(_partUpdateTextStyle)")
		};
	}/*namespace*/
	void UpdateTextStyle::load() {
		arx_add_main_command<UpdateTextStyle>();
		arx_add_main_command<UpdateTextStyle_1>();
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
		
		template<unsigned long long Version>
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
				argR->setBigFontFileName(LR"(dxhz.shx)")/*shx大字体文件名*/;

				auto varSetTextHeight = [argR]() {
					argR->setTextSize(5.7)/*文字高度*/;
					if (argR->priorSize() < 5) {
						argR->setPriorSize(5.7)/*默认大小*/;
					}
				};

				if constexpr(Version == 0) {
					varSetTextHeight();
				}
				else {
					if (varLocalR) {
						varSetTextHeight();
					}
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
#include "appendStyle/@仿宋_5.7_1.hpp"
			return std::move(varAns);
		}

		template<long long Version>
		inline void _p_update_text_style(AcDbDatabase * argDB) {
			
			sstd::ArxClosePointer< AcDbTextStyleTableRecord > varCopyOfStyleStandard{
				new AcDbTextStyleTableRecord
			};
			bool varCopyStandard = false;
			bool varHasStandard_1 = false;

			sstd::ArxClosePointer< AcDbTextStyleTable > varTextStyleTable;
			if (Acad::eOk != argDB->getTextStyleTable(varTextStyleTable, AcDb::kForWrite)) {
				acutPrintf(LR"(获得文字样式失败
)"); return;
			}

			auto varFunctions = _p_createFunctions<Version>();
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
					ArxString varTextStyleName ;
					if (Acad::eOk != varTextStyleTableRecord->getName(varTextStyleName.pointer())) {
						continue;
					}
					
					const std::wstring_view varTextStyleNameW(varTextStyleName.pointer());

					{
						if ( varTextStyleNameW == LR"(Standard)"sv ) {
							varCopyOfStyleStandard->copyFrom(
								varTextStyleTableRecord.pointer());
							varCopyStandard = true;
						}
						else if(varTextStyleNameW == LR"(Standard_1)"sv){
							varHasStandard_1 = true;
						}
					}

					auto varPos = varFunctions.find(varTextStyleNameW);
					if (varPos == varNoPos) { continue; }

					if constexpr(Version == 0) {
						acutPrintf(LR"(已有文字样式：)");
						acutPrintf(varTextStyleName);
						acutPrintf(LR"(
)");
					}

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

			if (varCopyStandard&&(false == varHasStandard_1)) {
				AcDbObjectId varTmpID;
				varCopyOfStyleStandard->setName(LR"(Standard_1)");
				varTextStyleTable->add(varTmpID,varCopyOfStyleStandard.pointer());
			}

		}
	}/*namespace*/

	void UpdateTextStyle::main() {
		_p_update_text_style<0>(acdbHostApplicationServices()->workingDatabase());
	}

	void UpdateTextStyle::main_do_not_change_text_height(){
		_p_update_text_style<1>(acdbHostApplicationServices()->workingDatabase());
	}

	extern void loadUpdateTextStyle() { UpdateTextStyle::load(); }

}/*namespace sstd*/

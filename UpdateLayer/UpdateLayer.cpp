#include <map>
#include <optional>
#include "UpdateLayer.hpp"

namespace sstd {

	extern void loadUpdateLayer() { UpdateLayer::load(); }

	UpdateLayer::UpdateLayer() {
	}

	void UpdateLayer::load() {
		arx_add_main_command<UpdateLayer>();
	}

	namespace {
#define simple_code_args const std::wstring_view & argNM , AcDbLayerTable * argLT, AcDbLinetypeTable * argLTT , AcDbLayerTableRecord * argLTR
		using ApplyLayerType = void(*)(simple_code_args);
		using ApplyMaps = std::map<std::wstring_view, std::pair<ApplyLayerType, bool> >;
		inline ApplyMaps _p_createFunctions() {
			ApplyMaps varAns;
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			varAns.emplace(LR"(0)"sv, ApplyMaps::value_type::second_type{ [](simple_code_args) {
				sstd::ArxClosePointer<AcDbLayerTableRecord> varLocalLTR;
				if (argLTR == nullptr) {
					varLocalLTR = new AcDbLayerTableRecord;
					argLTR = varLocalLTR;
					argLTR->setName(argNM.data());
				}

				/*初始化颜色*****************************************************/
				AcCmColor varLColor;
				varLColor.setColorIndex(211);
				/*初始化线宽*****************************************************/
				AcDb::LineWeight varLWeight = AcDb::kLnWt030;
				/*设置透明度*****************************************************/
				AcCmTransparency varLTP;
				varLTP.setAlphaPercent(1.0)/*1.0代表不透明，0.0代表透明*/;
				/*初始化线型*****************************************************/
				std::optional<AcDbObjectId> varLTypeID;
				{
					AcDbObjectId varLTypeIDTmp;
					if (Acad::eOk == argLTT->getAt(LR"(Continuous)", varLTypeIDTmp)) {
						varLTypeID = varLTypeIDTmp;
					}
					else {
						acutPrintf(LR"(找不到线型：Continuous
)");
					}
				}
				/******************************************************/
				argLTR->setIsOff(false)/*打开/关闭*/;
				argLTR->setIsFrozen(false)/*冻结*/;
				argLTR->setIsLocked(false)/*锁定*/;
				argLTR->setColor(varLColor)/*颜色*/;
				argLTR->setIsPlottable(true)/*打印*/;
				argLTR->setDescription(LR"(图层：0)")/*注释*/;
				if (varLTypeID) {
					argLTR->setLinetypeObjectId(*varLTypeID)/*设置线型*/;
				}
				else {
					acutPrintf(LR"(采用默认线型
)");
				}
				argLTR->setLineWeight(varLWeight)/*线宽*/;
				argLTR->setTransparency(varLTP)/*透明度*/;

				if (varLocalLTR) {
					argLT->add(argLTR);
				}

			} ,false });
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			varAns.emplace(LR"(Defpoints)"sv, ApplyMaps::value_type::second_type{ [](simple_code_args) {
				sstd::ArxClosePointer<AcDbLayerTableRecord> varLocalLTR;
				if (argLTR == nullptr) {
					varLocalLTR = new AcDbLayerTableRecord;
					argLTR = varLocalLTR;
					argLTR->setName(argNM.data());
				}

				/*初始化颜色*****************************************************/
				AcCmColor varLColor;
				varLColor.setRGB(
					std::uint8_t(185),
					std::uint8_t(70),
					std::uint8_t(173));
				/*初始化线宽*****************************************************/
				AcDb::LineWeight varLWeight = AcDb::kLnWt015;
				/*设置透明度*****************************************************/
				AcCmTransparency varLTP;
				varLTP.setAlphaPercent(1.0 - 0.45)/*1.0代表不透明，0.0代表透明*/;
				/*初始化线型*****************************************************/
				std::optional<AcDbObjectId> varLTypeID;
				{
					AcDbObjectId varLTypeIDTmp;
					if (Acad::eOk == argLTT->getAt(LR"(Continuous)", varLTypeIDTmp)) {
						varLTypeID = varLTypeIDTmp;
					}
					else {
						acutPrintf(LR"(找不到线型：Continuous
)");
					}
				}
				/******************************************************/
				argLTR->setIsOff(false)/*打开/关闭*/;
				argLTR->setIsFrozen(false)/*冻结*/;
				argLTR->setIsLocked(false)/*锁定*/;
				argLTR->setColor(varLColor)/*颜色*/;
				argLTR->setIsPlottable(false)/*打印*/;
				argLTR->setDescription(LR"(图层：Defpoints)")/*注释*/;
				if (varLTypeID) {
					if (Acad::eOk!= argLTR->setLinetypeObjectId(*varLTypeID)/*设置线型*/) {
						acutPrintf(LR"(设置线型失败
)");
					}
				}
				else {
					acutPrintf(LR"(采用默认线型
)");
				}
				argLTR->setLineWeight(varLWeight)/*线宽*/;
				argLTR->setTransparency(varLTP)/*透明度*/;

				if (varLocalLTR) {
					argLT->add(argLTR);
				}

			} ,false });
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*01*/#include "appendLayer/BZ.hpp"
/*02*/#include "appendLayer/标注.hpp"
/*03*/#include "appendLayer/尺寸线.hpp"
/*04*/#include "appendLayer/尺寸线层.hpp"
/*05*/#include "appendLayer/不打印.hpp"
/*06*/#include "appendLayer/图层1.hpp"
/*07*/#include "appendLayer/图层2.hpp"
/*08*/#include "appendLayer/参考点.hpp"
/*09*/#include "appendLayer/参考线.hpp"
/*10*/#include "appendLayer/粗实线.hpp"
/*11*/#include "appendLayer/粗实线层.hpp"
/*12*/#include "appendLayer/粗实线_粗加工.hpp"
/*13*/#include "appendLayer/粗实线_非机械加工.hpp"
/*14*/#include "appendLayer/细实线层.hpp"
/*15*/#include "appendLayer/细实线.hpp"
/*16*/#include "appendLayer/剖面线层.hpp"
/*17*/#include "appendLayer/TXT.hpp"
/*18*/#include "appendLayer/图纸界限.hpp"
/*19*/#include "appendLayer/_MarkInformation.hpp"
/*20*/#include "appendLayer/_坐标.hpp"
/*21*/#include "appendLayer/虚线.hpp"
/*21*/#include "appendLayer/虚线层.hpp"
/*22*/#include "appendLayer/双点划线.hpp"
/*23*/#include "appendLayer/中心线.hpp"
/*24*/#include "appendLayer/中心线层.hpp"
/*25*/#include "appendLayer/BIAOZHU.hpp"
/*26*/#include "appendLayer/临时图形.hpp"
/*27*/#include "appendLayer/隐藏层.hpp"
			return std::move(varAns);
		}

		inline void _p_update_layer(AcDbDatabase * argDB) {
			if (nullptr == argDB) { return; }

			/*加载线型*/
			argDB->loadLineTypeFile(LR"(Continuous)", LR"(acad.lin)");
			argDB->loadLineTypeFile(LR"(CENTER2)", LR"(acad.lin)");
			argDB->loadLineTypeFile(LR"(PHANTOM2)", LR"(acad.lin)");
			argDB->loadLineTypeFile(LR"(DASHED)", LR"(acad.lin)");

			/*获得线型表*/
			sstd::ArxClosePointer< AcDbLinetypeTable > varLinetypeTable;
			if (Acad::eOk != argDB->getLinetypeTable(
				varLinetypeTable, AcDb::kForRead)) {
				acutPrintf(LR"(获得线型失败
)");
				return;
			}

			/*获得图层表*/
			sstd::ArxClosePointer< AcDbLayerTable > varLayerTable;
			if (Acad::eOk != argDB->getLayerTable(
				varLayerTable, AcDb::kForWrite)) {
				acutPrintf(LR"(获得线型失败
)");
				return;
			}

			/**/
			auto varFunctions = _p_createFunctions();
			const auto varFNoPos = varFunctions.end();

			/*处理已经存在图层*/
			{
				std::unique_ptr<AcDbLayerTableIterator> varIt;
				{
					AcDbLayerTableIterator * varItTmp = nullptr;
					if (Acad::eOk != varLayerTable->newIterator(varItTmp)) {
						acutPrintf(LR"(图层迭代失败
)");
						return;
					}
					varIt.reset(varItTmp);
				}

				for (varIt->start(); !varIt->done(); varIt->step()) {
					sstd::ArxClosePointer< AcDbLayerTableRecord > varLTR;
					if (Acad::eOk == varIt->getRecord(varLTR, AcDb::kForWrite)) {
						wchar_t * varName = nullptr;
						if (Acad::eOk == varLTR->getName(varName)) {
							auto varCPos = varFunctions.find(varName);
							if (varCPos == varFNoPos) { continue; }
							varCPos->second.second = true;
							varCPos->second.first(varCPos->first,
								varLayerTable,
								varLinetypeTable,
								varLTR
							);
						}
					}
				}
			}

			/*创建新图层*/
			for (auto & varI : varFunctions) {
				if (varI.second.second == false) {
					varI.second.second = true;
					varI.second.first(varI.first,
						varLayerTable,
						varLinetypeTable,
						nullptr);
				}
			}

		}
	}/*namespace*/

	void UpdateLayer::main() {
		_p_update_layer(acdbHostApplicationServices()->workingDatabase());
	}

}/*namespace sstd*/




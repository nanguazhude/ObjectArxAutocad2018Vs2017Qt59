
#include "FVAangleLine.hpp"
#include <cmath>
#include <vector>
#include <array>
#include <optional>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
}

namespace sstd {
	extern std::array<std::uint8_t, 3> randColr();
}

namespace {
	inline constexpr const wchar_t * const _ref_layerName() {
		return LR"(参考线)";
	}
	inline constexpr const wchar_t * const _this_layerName() {
		return LR"(粗实线)";
	}
	inline void set_line_not_r(AcDbLine * l, const AcCmColor&) {
		l->setLayer(_this_layerName());
	}
	inline void set_line_r(AcDbLine * l, const AcCmColor&c) {
		l->setLayer(_ref_layerName());
		l->setColor(c);
	}
}

namespace {
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {
	extern void loadFVAngleLine() {
		FVAngleLine::load();
	}

	void FVAngleLine::load() {
		arx_add_main_command<FVAngleLine>();
	}

	namespace {
		namespace _ns_pp0 {
			class Main {
			public:

				AcDbDatabase * $DB;
				AcDbObjectId $SLayerID;
				double $HDist;
				double $VDist1;
				double $VDist2;
				double $Angle;
				AcGePoint3d $StartPoint;
				int $Error = 0;

				std::array<AcGePoint3d, 9> $Points;
				enum : int {
					StartPoint = 0,
					StartPointHigh = 1,
					StartPointLow = 2,

					MiddlePoint = 3,
					MiddlePointHigh = 4,
					MiddlePointLow = 5,

					EndPoint = 6,
					EndPointHigh = 7,
					EndPointLow = 8,
				};

				AcDbObjectId $S_SH;
				AcDbObjectId $S_SL;
				AcDbObjectId $M_MH;
				AcDbObjectId $M_ML;
				AcDbObjectId $E_EH;
				AcDbObjectId $E_EL;
				AcDbObjectId $S_M;
				AcDbObjectId $M_E;
				AcDbObjectId $MH_EH;
				AcDbObjectId $ML_EL;
				AcDbObjectId $SH_MH;
				AcDbObjectId $SL_ML;

				AcCmColor $Color;

				inline Main();
				inline ~Main();

				inline void getStartPoint();
				inline void getVDist1();
				inline void getVDist2();
				inline void getHDist();
				inline void getAngle();
				inline void exec();
				inline void draw();
				inline void constraint();

				inline void check_error(int);
			};

			inline void Main::check_error(int varG) {
				if ($Error == varG) { return; }
				throw $Error;
			}

			inline Main::Main() {
				const auto varRColor = sstd::randColr();
				$Color.setRGB(varRColor[0], varRColor[1], varRColor[2]);
				$DB = acdbHostApplicationServices()->workingDatabase();
				$SLayerID = $DB->clayer();
				$DB->setClayer($DB->layerZero());
			}

			inline Main::~Main() {
				$DB->setClayer($SLayerID);
			}

			inline void Main::getVDist1() {
				$Error = acedGetDist(nullptr, LR"(请输入垂直距离1<15.3>：)", &$VDist1);
				if (RTNONE == $Error) { $VDist1 = 15.3; }
				else { check_error(RTNORM); }
			}

			inline void Main::getVDist2() {
				$Error = acedGetDist(nullptr, LR"(请输入垂直距离2<5.3>：)", &$VDist2);
				if (RTNONE == $Error) { $VDist2 = 5.3; }
				else { check_error(RTNORM); }
			}

			inline void Main::getHDist() {
				$Error = acedGetDist(nullptr, LR"(请输入水平距离<15.3>：)", &$HDist);
				if (RTNONE == $Error) { $HDist = 15.3; }
				else { check_error(RTNORM); }
			}

			inline void Main::getAngle() {
				$Error = acedGetReal(LR"(请输入角度<90>：)", &$Angle);
				if (RTNONE == $Error) { $Angle = 90; }
				else { check_error(RTNORM); }
			}

			inline void Main::getStartPoint() {
				$Error = acedGetPoint(nullptr, LR"(请输入起始中心<0,0>：)", &($StartPoint.x));
				if (RTNONE == $Error) { $StartPoint = { 0.0,0.0,0.0 }; }
				else { check_error(RTNORM); }
				$StartPoint = UcsToWorld($StartPoint);
			}

			inline void Main::exec()try {

				$VDist1 = std::abs($VDist1);
				$VDist2 = std::abs($VDist2);
				if ($VDist2 == 0) { throw LR"(if ($VDist2 == 0))"; }
				if ($VDist1 == 0) { throw LR"(if ($VDist1 == 0))"; }
				if ($VDist1 == $VDist2) { LR"(if ($VDist1 == $VDist2))"; }
				if ($HDist == 0) { LR"(if ($HDist == 0) )"; }
				$Angle = std::abs($Angle);
				if ($Angle == 0) { LR"(if ($Angle == 0) )"; }
				if ($Angle >= 360) { LR"(if ($Angle >= 360))"; }
				if ($Angle == 180) { LR"(if ($Angle == 180))"; }

				{
					$Points[StartPoint] = $StartPoint;
					$Points[StartPointHigh] = $StartPoint;
					$Points[StartPointLow] = $StartPoint;
					const auto V1Half = 0.5*$VDist1;
					{
						$Points[StartPointHigh].y += V1Half;
						$Points[StartPointLow].y -= V1Half;
					}
				}

				{
					$Points[EndPoint] = $StartPoint;
					$Points[EndPoint].x += $HDist;
					const auto V2Half = 0.5*$VDist2;
					$Points[EndPointHigh] = $Points[EndPoint];
					$Points[EndPointLow] = $Points[EndPoint];
					{
						$Points[EndPointHigh].y += V2Half;
						$Points[EndPointLow].y -= V2Half;
					}
				}

				constexpr const auto pi = 3.141592654;
				constexpr const auto div_180 = 0.5 / 180.0;
				constexpr const auto K = div_180 * pi;

				const bool V1BV2 = $VDist1 > $VDist2;
				const auto VV1V2 = V1BV2 ?
					($VDist1 - $VDist2) * (std::signbit($HDist) ? -0.5 : 0.5) :
					($VDist2 - $VDist1) * (std::signbit($HDist) ? -0.5 : 0.5);

				/* 0-90 */
				const auto  HV1V2 = VV1V2 * std::tan(K*$Angle);

				if (V1BV2) {
					$Points[MiddlePoint] = $Points[StartPoint];
					$Points[MiddlePoint].x += HV1V2;
					$Points[MiddlePointHigh] = $Points[MiddlePoint];
					$Points[MiddlePointLow] = $Points[MiddlePoint];
					$Points[MiddlePointHigh].y = $Points[EndPointHigh].y;
					$Points[MiddlePointLow].y = $Points[EndPointLow].y;
				}
				else {
					$Points[MiddlePoint] = $Points[EndPoint];
					$Points[MiddlePoint].x -= HV1V2;
					$Points[MiddlePointHigh] = $Points[MiddlePoint];
					$Points[MiddlePointLow] = $Points[MiddlePoint];
					$Points[MiddlePointHigh].y = $Points[StartPointHigh].y;
					$Points[MiddlePointLow].y = $Points[StartPointLow].y;
				}

				/******************************************/
				/*{
					QString varDebugData;
					{
						QTextStream varStream(&varDebugData);
						for (const auto & varI : $Points) {
							varStream << varI.x << " , " << varI.y << endl;
						}
					}
					const auto s = varDebugData.toStdWString();
					acutPrintf(s.c_str());
				}*/
				/******************************************/

			}
			catch (const wchar_t * arg) {
				acutPrintf(arg);
				acutPrintf(LR"(
)");
				throw arg;
			}
			catch (...) {
				acutPrintf(LR"(quit @ exec!
)");
				throw;
			}

			inline void Main::draw() {
				sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
				sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
				std::vector< sstd::ArxClosePointer< AcDbLine > > varLines;
				varLines.reserve(36);

				{
					auto varE = $DB->getBlockTable(varBlockTable, AcDb::kForRead);
					if (varE != eOk) { throw varE; }
					varE = varBlockTable->getAt(ACDB_MODEL_SPACE,
						varBlockTableRecord,
						AcDb::kForWrite);
					if (varE != eOk) { throw varE; }
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[StartPoint], $Points[MiddlePoint]));
					varBlockTableRecord->appendAcDbEntity($S_M, varLine);
					set_line_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[MiddlePoint], $Points[EndPoint]));
					varBlockTableRecord->appendAcDbEntity($M_E, varLine);
					set_line_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[StartPoint], $Points[StartPointHigh]));
					varBlockTableRecord->appendAcDbEntity($S_SH, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[StartPoint], $Points[StartPointLow]));
					varBlockTableRecord->appendAcDbEntity($S_SL, varLine);
					set_line_not_r(varLine, $Color);
				}


				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[MiddlePoint], $Points[MiddlePointHigh]));
					varBlockTableRecord->appendAcDbEntity($M_MH, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[MiddlePoint], $Points[MiddlePointLow]));
					varBlockTableRecord->appendAcDbEntity($M_ML, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[EndPoint], $Points[EndPointHigh]));
					varBlockTableRecord->appendAcDbEntity($E_EH, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[EndPoint], $Points[EndPointLow]));
					varBlockTableRecord->appendAcDbEntity($E_EL, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[StartPointLow], $Points[MiddlePointLow]));
					varBlockTableRecord->appendAcDbEntity($SL_ML, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[MiddlePointLow], $Points[EndPointLow]));
					varBlockTableRecord->appendAcDbEntity($ML_EL, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[StartPointHigh], $Points[MiddlePointHigh]));
					varBlockTableRecord->appendAcDbEntity($SH_MH, varLine);
					set_line_not_r(varLine, $Color);
				}

				{
					auto & varLine =
						varLines.emplace_back(new AcDbLine($Points[MiddlePointHigh], $Points[EndPointHigh]));
					varBlockTableRecord->appendAcDbEntity($MH_EH, varLine);
					set_line_not_r(varLine, $Color);
				}

			}

			inline void Main::constraint() {
				/*重合约束*/
				{
					auto f = [](const auto & l1, const auto & l2, const auto & p1) {
						AcDbAssoc2dConstraintAPI::createCoincidentConstraint(l1, l2, p1, p1);
					};
					f($S_SH, $S_M, $Points[StartPoint]);
					f($S_SL, $S_M, $Points[StartPoint]);

					f($S_M, $M_E, $Points[MiddlePoint]);
					f($S_M, $M_MH, $Points[MiddlePoint]);
					f($S_M, $M_ML, $Points[MiddlePoint]);

					f($M_E, $E_EH, $Points[EndPoint]);
					f($M_E, $E_EL, $Points[EndPoint]);

					f($S_SH, $SH_MH, $Points[StartPointHigh]);
					f($S_SL, $SL_ML, $Points[StartPointLow]);

					f($MH_EH, $E_EH, $Points[EndPointHigh]);
					f($ML_EL, $E_EL, $Points[EndPointLow]);

					f($M_MH, $SH_MH, $Points[MiddlePointHigh]);
					f($M_MH, $MH_EH, $Points[MiddlePointHigh]);

					f($M_ML, $SL_ML, $Points[MiddlePointLow]);
					f($M_ML, $ML_EL, $Points[MiddlePointLow]);
				}
				/*水平垂直约束*/
				{
					auto h = [](const auto &l, const auto & p) {
						AcDbAssoc2dConstraintAPI::createHorizontalConstraint(l, p);
					};
					auto v = [](const auto & l, const auto & p) {
						AcDbAssoc2dConstraintAPI::createVerticalConstraint(l, p);
					};

					v($S_SH, $Points[StartPoint]);
					v($S_SL, $Points[StartPoint]);
					v($M_MH, $Points[MiddlePoint]);
					v($M_ML, $Points[MiddlePoint]);
					v($E_EH, $Points[EndPoint]);
					v($E_EL, $Points[EndPoint]);
					h($S_M, $Points[MiddlePoint]);
					h($M_E, $Points[MiddlePoint]);
				}
				/*相等约束*/
				{
					auto f = [](const auto & l1, const auto & l2, const auto & p1) {
						AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(l1, l2, p1, p1);
					};
					f($S_SH, $S_SL, $Points[StartPoint]);
					f($M_MH, $M_ML, $Points[MiddlePoint]);
					f($E_EL, $E_EH, $Points[EndPoint]);
				}

				AcDbObjectId varTmp;
				AcGePoint3d varDimPos;
				/*角度约束*/
				{
					auto f = [&varTmp, &varDimPos](const auto & l1, const auto &p1,
						const auto &l2, const auto &p2) {
						AcDbAssoc2dConstraintAPI::create2LineAngularDimConstraint(
							l1, l2, p1, p2, varDimPos, varTmp);
					};
					if ($Points[StartPointHigh].y == $Points[MiddlePointHigh].y) {
						if (std::signbit($HDist)) {
							varDimPos = $Points[EndPoint];
							f($ML_EL, $Points[EndPointLow], $MH_EH, $Points[EndPointHigh]);
						}
						else {
							varDimPos = $Points[EndPoint];
							f($MH_EH, $Points[EndPointHigh], $ML_EL, $Points[EndPointLow]);
						}
					}
					else {
						if (std::signbit($HDist)) {
							varDimPos = $Points[StartPoint];
							f($SH_MH, $Points[StartPointHigh], $SL_ML, $Points[StartPointLow]);
						}
						else {
							varDimPos = $Points[StartPoint];
							f($SL_ML, $Points[StartPointLow], $SH_MH, $Points[StartPointHigh]);
						}
					}
				}

				///*线性约束*/
				//{
				//	auto f = [&varTmp, &varDimPos](const auto & l, const auto &p0, const auto & p1) {
				//		AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(l, p0, p1, varDimPos, varTmp);
				//		AcString varName;
				//		AcDbEvalVariant varValue;
				//		AcString varExpression;
				//		AcString varEvaluatorId;
				//		AcString varErrorString;
				//		AcDbAssoc2dConstraintAPI::getVariableValue(
				//			varTmp,
				//			varName,
				//			varValue,
				//			varExpression,
				//			varEvaluatorId
				//		);

				//		{
				//			const auto Expression_ = QString::number(std::abs(
				//				p0.y - p1.y)*2.) +
				//				QLatin1String(R"(/2)", 2);
				//			const auto varTS = Expression_.toStdWString();
				//			varExpression = AcString{ varTS.c_str() };
				//		}

				//		AcDbAssoc2dConstraintAPI::setVariableValue(varTmp,
				//			varValue,
				//			varExpression,
				//			varEvaluatorId,
				//			varErrorString);

				//	};

				//	f($S_SH, $Points[StartPoint], $Points[StartPointHigh]);
				//	f($S_SL, $Points[StartPoint], $Points[StartPointLow]);
				//}
			}
		}/*namespace _ns_pp0*/
	}/*namespace*/

	void FVAngleLine::main()try {

		auto varThis = std::make_unique< _ns_pp0::Main >();
		/*获得起始点*/
		varThis->getStartPoint();
		/*获得水平距离*/
		varThis->getHDist();
		/*获得垂直距离1*/
		varThis->getVDist1();
		/*获得垂直距离2*/
		varThis->getVDist2();
		/*输入角度*/
		varThis->getAngle();
		/*计算*/
		varThis->exec();
		/*绘图*/
		varThis->draw();
		/*增加约束*/
		varThis->constraint();
	}
	catch (...) {
		return;
	}


}


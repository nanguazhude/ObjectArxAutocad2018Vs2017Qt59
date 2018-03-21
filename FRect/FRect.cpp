#include "FRect.hpp"
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
	inline constexpr const wchar_t * const _this_layerName() {
		return LR"(参考线)";
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
	FRect::FRect() {

	}
	extern void loadFRect() {
		FRect::load();
	}
	void FRect::load() {
		arx_add_main_command<FRect>();
	}

	namespace {
		namespace _p000_ {

			class Rect {
			public:
				double $Width;
				double $Height;
				Rect(double a, double b) :$Width(a), $Height(b) {}
				Rect() :$Width(0), $Height(0) {}
			};

			class PrivatePack {
			public:
				AcGePoint3d $StartPoint;
				AcDbDatabase * $DB = nullptr;
				int $Error = 0;
				Rect $Rects;
			};

			inline void check_error(PrivatePack * arg, int varG) {
				if (arg->$Error == varG) { return; }
				throw arg->$Error;
			}

			inline bool _p_construct_pack(PrivatePack * arg) {
				arg->$DB = acdbHostApplicationServices()->workingDatabase();
				return true;
			}

			inline bool _p_select_point(PrivatePack * arg) try {
				arg->$Error = acedGetPoint(nullptr, LR"(请输入起始中心<0,0>：)", &(arg->$StartPoint.x));
				if (RTNONE == arg->$Error) { arg->$StartPoint = { 0.0,0.0,0.0 }; }
				else { check_error(arg, RTNORM); }
				return true;
			}
			catch (...) {
				return false;
			}

			inline void _p_get_y(PrivatePack * arg) {
				//arg->$Error = acedGetReal(LR"(请输入高度<15.3>：)", &arg->$Rects.$Height);
				arg->$Error = acedGetDist(nullptr, LR"(请输入高度<15.3>：)", &arg->$Rects.$Height);
				if (RTNONE == arg->$Error) { arg->$Rects.$Height = 15.3; }
				else { check_error(arg, RTNORM); }
			}

			inline void _p_get_x(PrivatePack * arg) {
				//arg->$Error = acedGetReal(LR"(请输入宽度<15.3>：)", &arg->$Rects.$Width);
				arg->$Error = acedGetDist(nullptr, LR"(请输入宽度<15.3>：)", &arg->$Rects.$Width);
				if (RTNONE == arg->$Error) { arg->$Rects.$Width = 15.3; }
				else { check_error(arg, RTNORM); }
			}

			class DrawARectPack {
			public:
				AcGePoint3d varKeyPoint0;
				AcGePoint3d varKeyPoint1;
				AcGePoint3d varKeyPoint2;
				AcGePoint3d varKeyPoint3;
				AcGePoint3d varKeyPoint4;
				AcGePoint3d varKeyPoint5;

				AcGePoint3d varKeyPoint0_1;
				AcGePoint3d varKeyPoint1_2;
				AcGePoint3d varKeyPoint2_3;
				AcGePoint3d varKeyPoint3_4;
				AcGePoint3d varKeyPoint4_5;
				AcGePoint3d varKeyPoint5_0;

				using LineType = sstd::ArxClosePointer< AcDbLine >;
				std::array<LineType, 6> varLines;
				sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
				sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

				AcDbObjectId varID0;
				AcDbObjectId varID1;
				AcDbObjectId varID2;
				AcDbObjectId varID3;
				AcDbObjectId varID4;
				AcDbObjectId varID5;

				std::vector<AcDbObjectId> varCObjecs;
				AcCmColor varColor;
			};

			inline auto add_scale_half(const double & a, const double &b) {
				return (a + b)*0.5;
			}

			inline auto mid(const AcGePoint3d & x, const AcGePoint3d &y) {
				return AcGePoint3d(add_scale_half(x.x, y.x),
					add_scale_half(x.y, y.y),
					0.
				);
			}

			inline void _p_draw_a_rect(
				const Rect & argR,
				DrawARectPack & argP,
				PrivatePack & arg) {

					{
						auto varRC = sstd::randColr();
						argP.varColor.setRGB(varRC[0], varRC[1], varRC[2]);
					}

					{
						arg.$Error = arg.$DB->getBlockTable(argP.varBlockTable, AcDb::kForRead);
						check_error(&arg, eOk);
						arg.$Error = argP.varBlockTable->getAt(ACDB_MODEL_SPACE, argP.varBlockTableRecord,
							AcDb::kForWrite);
						check_error(&arg, eOk);
					}

					const auto & varW = argR.$Width;
					const double varHH = argR.$Height*0.5;

					argP.varKeyPoint0 = arg.$StartPoint;
					argP.varKeyPoint1 = arg.$StartPoint;
					argP.varKeyPoint1.y += varHH;
					argP.varKeyPoint2 = argP.varKeyPoint1;
					argP.varKeyPoint2.x -= varW;
					argP.varKeyPoint3 = arg.$StartPoint;
					argP.varKeyPoint3.x = argP.varKeyPoint2.x;
					argP.varKeyPoint4 = argP.varKeyPoint3;
					argP.varKeyPoint4.y -= varHH;
					argP.varKeyPoint5 = argP.varKeyPoint4;
					argP.varKeyPoint5.x = argP.varKeyPoint0.x;

					argP.varKeyPoint0 = UcsToWorld(argP.varKeyPoint0);
					argP.varKeyPoint1 = UcsToWorld(argP.varKeyPoint1);
					argP.varKeyPoint2 = UcsToWorld(argP.varKeyPoint2);
					argP.varKeyPoint3 = UcsToWorld(argP.varKeyPoint3);
					argP.varKeyPoint4 = UcsToWorld(argP.varKeyPoint4);
					argP.varKeyPoint5 = UcsToWorld(argP.varKeyPoint5);

					argP.varLines[0] = { new AcDbLine{ argP.varKeyPoint0 ,argP.varKeyPoint1 } };
					argP.varLines[1] = { new AcDbLine{ argP.varKeyPoint1 ,argP.varKeyPoint2 } };
					argP.varLines[2] = { new AcDbLine{ argP.varKeyPoint2 ,argP.varKeyPoint3 } };
					argP.varLines[3] = { new AcDbLine{ argP.varKeyPoint3 ,argP.varKeyPoint4 } };
					argP.varLines[4] = { new AcDbLine{ argP.varKeyPoint4 ,argP.varKeyPoint5 } };
					argP.varLines[5] = { new AcDbLine{ argP.varKeyPoint5 ,argP.varKeyPoint0 } };

					argP.varKeyPoint0_1 = mid(argP.varKeyPoint0, argP.varKeyPoint1);
					argP.varKeyPoint1_2 = mid(argP.varKeyPoint1, argP.varKeyPoint2);
					argP.varKeyPoint2_3 = mid(argP.varKeyPoint2, argP.varKeyPoint3);
					argP.varKeyPoint3_4 = mid(argP.varKeyPoint3, argP.varKeyPoint4);
					argP.varKeyPoint4_5 = mid(argP.varKeyPoint4, argP.varKeyPoint5);
					argP.varKeyPoint5_0 = mid(argP.varKeyPoint5, argP.varKeyPoint0);

					/*添加线到数据库*/
					{
						int i = 0;
						for (auto & varI : argP.varLines) {
							switch (i) {
							case 0: {
								argP.varBlockTableRecord->appendAcDbEntity(argP.varID0, varI);
								argP.varCObjecs.push_back(argP.varID0);
							} break;
							case 1:argP.varBlockTableRecord->appendAcDbEntity(argP.varID1, varI); break;
							case 2:argP.varBlockTableRecord->appendAcDbEntity(argP.varID2, varI); break;
							case 3:argP.varBlockTableRecord->appendAcDbEntity(argP.varID3, varI); break;
							case 4:argP.varBlockTableRecord->appendAcDbEntity(argP.varID4, varI); break;
							case 5:argP.varBlockTableRecord->appendAcDbEntity(argP.varID5, varI); break;
							}
							++i;
						}
					}

					/*close */
					for (auto & varI : argP.varLines) {
						varI->setLayer(_this_layerName());
						varI->setColor(argP.varColor);
						varI = {};
					}
					argP.varBlockTableRecord = {};
					argP.varBlockTable = {};

			}

			inline void _p_constraint(DrawARectPack & argP) {

				/*添加重合约束*/
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID0, argP.varID1, argP.varKeyPoint1, argP.varKeyPoint1);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID1, argP.varID2, argP.varKeyPoint2, argP.varKeyPoint2);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID2, argP.varID3, argP.varKeyPoint3, argP.varKeyPoint3);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID3, argP.varID4, argP.varKeyPoint4, argP.varKeyPoint4);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID4, argP.varID5, argP.varKeyPoint5, argP.varKeyPoint5);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID5, argP.varID0, argP.varKeyPoint0, argP.varKeyPoint0);

				/*添加水平，垂直约束*/
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID1, argP.varKeyPoint1_2);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID4, argP.varKeyPoint4_5);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID0, argP.varKeyPoint0_1);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID2, argP.varKeyPoint2_3);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID3, argP.varKeyPoint3_4);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID5, argP.varKeyPoint5_0);

				/*添加相同约束*/
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID0, argP.varID5, argP.varKeyPoint0_1, argP.varKeyPoint5_0);
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID2, argP.varID3, argP.varKeyPoint2_3, argP.varKeyPoint3_4);

				/*添加尺寸约束*/
				AcDbObjectId varTmp;
				const auto varCPosR = mid(argP.varKeyPoint1, argP.varKeyPoint4);
				const AcGePoint3d & varCPos = varCPosR;
				AcDbAssoc2dConstraintAPI::createHorizontalDimConstraint(argP.varID0, argP.varID2,
					argP.varKeyPoint0_1, argP.varKeyPoint2_3, varCPos, varTmp);
				AcDbAssoc2dConstraintAPI::createVerticalDimConstraint(argP.varID1, argP.varID4,
					argP.varKeyPoint1_2, argP.varKeyPoint4_5, varCPos, varTmp);
			}

		}
	}/*namespace*/

	void FRect::main() try {
		using namespace _p000_;

		PrivatePack varData;

		/*construc pack*/
		if (false == _p_construct_pack(&varData)) {
			return;
		}

		class Lock {
			PrivatePack * d;
			AcDbObjectId layerID;
			//AcGeMatrix3d ucs;
		public:
			Lock(PrivatePack *v) :d(v) {
				layerID = d->$DB->clayer();
				d->$DB->setClayer(d->$DB->layerZero());
			}
			~Lock() {
				d->$DB->setClayer(layerID);
			}
		};
		Lock __lock{ &varData };

		/*select point*/
		if (false == _p_select_point(&varData)) {
			return;
		}

		/*get width*/
		_p_get_y(&varData);
		/*get height*/
		_p_get_x(&varData);

		DrawARectPack varPackD;
		_p_draw_a_rect(varData.$Rects, varPackD, varData);
		_p_constraint(varPackD);
	}
	catch (...) {
		return;
	}

}/*namespace sstd*/








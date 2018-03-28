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
	extern AcCmColor smallChange(const AcCmColor &);
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
				AcDbDatabase * $DB = nullptr;
				AcGePoint3d $StartPoint;
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
				AcGePoint3d varKeyPoint6;
				AcGePoint3d varKeyPoint7;

				AcGePoint3d varKeyPoint0_1;
				AcGePoint3d varKeyPoint1_2;
				AcGePoint3d varKeyPoint2_3;
				AcGePoint3d varKeyPoint3_4;
				AcGePoint3d varKeyPoint4_5;
				AcGePoint3d varKeyPoint5_6;
				AcGePoint3d varKeyPoint6_7;
				AcGePoint3d varKeyPoint7_0;

				using LineType = sstd::ArxClosePointer< AcDbLine >;
				std::array<LineType, 8> varLines;
				sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
				sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

				AcDbObjectId varID0;
				AcDbObjectId varID1;
				AcDbObjectId varID2;
				AcDbObjectId varID3;
				AcDbObjectId varID4;
				AcDbObjectId varID5;
				AcDbObjectId varID6;
				AcDbObjectId varID7;

				//std::vector<AcDbObjectId> varCObjecs;
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
					const double varHW = argR.$Width*0.5;

					arg.$StartPoint = UcsToWorld(arg.$StartPoint);

					//0
					argP.varKeyPoint0 = arg.$StartPoint;
					//1
					argP.varKeyPoint1 = arg.$StartPoint;
					argP.varKeyPoint1.y += varHH;
					//2
					argP.varKeyPoint2 = argP.varKeyPoint1;
					argP.varKeyPoint2.x -= varHW;
					//3
					argP.varKeyPoint3 = argP.varKeyPoint1;
					argP.varKeyPoint3.x -= varW;
					//4
					argP.varKeyPoint4 = arg.$StartPoint;
					argP.varKeyPoint4.x = argP.varKeyPoint3.x;
					//5
					argP.varKeyPoint5 = argP.varKeyPoint4;
					argP.varKeyPoint5.y -= varHH;
					//6
					argP.varKeyPoint6 = argP.varKeyPoint5;
					argP.varKeyPoint6.x = argP.varKeyPoint2.x;
					//7
					argP.varKeyPoint7 = argP.varKeyPoint6;
					argP.varKeyPoint7.x = argP.varKeyPoint0.x;

					argP.varLines[0] = { new AcDbLine{ argP.varKeyPoint0 ,argP.varKeyPoint1 } };
					argP.varLines[1] = { new AcDbLine{ argP.varKeyPoint1 ,argP.varKeyPoint2 } };
					argP.varLines[2] = { new AcDbLine{ argP.varKeyPoint2 ,argP.varKeyPoint3 } };
					argP.varLines[3] = { new AcDbLine{ argP.varKeyPoint3 ,argP.varKeyPoint4 } };
					argP.varLines[4] = { new AcDbLine{ argP.varKeyPoint4 ,argP.varKeyPoint5 } };
					argP.varLines[5] = { new AcDbLine{ argP.varKeyPoint5 ,argP.varKeyPoint6 } };
					argP.varLines[6] = { new AcDbLine{ argP.varKeyPoint6 ,argP.varKeyPoint7 } };
					argP.varLines[7] = { new AcDbLine{ argP.varKeyPoint7 ,argP.varKeyPoint0 } };

					argP.varKeyPoint0_1 = mid(argP.varKeyPoint0, argP.varKeyPoint1);
					argP.varKeyPoint1_2 = mid(argP.varKeyPoint1, argP.varKeyPoint2);
					argP.varKeyPoint2_3 = mid(argP.varKeyPoint2, argP.varKeyPoint3);
					argP.varKeyPoint3_4 = mid(argP.varKeyPoint3, argP.varKeyPoint4);
					argP.varKeyPoint4_5 = mid(argP.varKeyPoint4, argP.varKeyPoint5);
					argP.varKeyPoint5_6 = mid(argP.varKeyPoint5, argP.varKeyPoint6);
					argP.varKeyPoint6_7 = mid(argP.varKeyPoint6, argP.varKeyPoint7);
					argP.varKeyPoint7_0 = mid(argP.varKeyPoint7, argP.varKeyPoint0);

					/*添加线到数据库*/
					{
						int i = 0;
						for (auto & varI : argP.varLines) {
							switch (i) {
							case 0: {
								argP.varBlockTableRecord->appendAcDbEntity(argP.varID0, varI);
								//argP.varCObjecs.push_back(argP.varID0);
							} break;
							case 1:argP.varBlockTableRecord->appendAcDbEntity(argP.varID1, varI); break;
							case 2:argP.varBlockTableRecord->appendAcDbEntity(argP.varID2, varI); break;
							case 3:argP.varBlockTableRecord->appendAcDbEntity(argP.varID3, varI); break;
							case 4:argP.varBlockTableRecord->appendAcDbEntity(argP.varID4, varI); break;
							case 5:argP.varBlockTableRecord->appendAcDbEntity(argP.varID5, varI); break;
							case 6:argP.varBlockTableRecord->appendAcDbEntity(argP.varID6, varI); break;
							case 7:argP.varBlockTableRecord->appendAcDbEntity(argP.varID7, varI); break;
							}
							++i;
						}
					}

					/*close */
					for (auto & varI : argP.varLines) {
						varI->setLayer(_this_layerName());
						varI->setColor(sstd::smallChange(argP.varColor));
						varI = {};
					}
					argP.varBlockTableRecord = {};
					argP.varBlockTable = {};

			}

			template<bool S>
			inline void _p_constraint(DrawARectPack & argP) {

				/*添加重合约束*/
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID0, argP.varID1, argP.varKeyPoint1, argP.varKeyPoint1);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID1, argP.varID2, argP.varKeyPoint2, argP.varKeyPoint2);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID2, argP.varID3, argP.varKeyPoint3, argP.varKeyPoint3);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID3, argP.varID4, argP.varKeyPoint4, argP.varKeyPoint4);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID4, argP.varID5, argP.varKeyPoint5, argP.varKeyPoint5);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID5, argP.varID6, argP.varKeyPoint6, argP.varKeyPoint6);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID6, argP.varID7, argP.varKeyPoint7, argP.varKeyPoint7);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID7, argP.varID0, argP.varKeyPoint0, argP.varKeyPoint0);
				/*添加水平，垂直约束*/
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID1, argP.varKeyPoint1_2);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID2, argP.varKeyPoint2_3);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID5, argP.varKeyPoint5_6);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID6, argP.varKeyPoint6_7);

				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID0, argP.varKeyPoint0_1);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID7, argP.varKeyPoint7_0);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID3, argP.varKeyPoint3_4);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID4, argP.varKeyPoint4_5);

				/*添加相同约束*/
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID0, argP.varID7, argP.varKeyPoint0_1, argP.varKeyPoint7_0);
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID1, argP.varID2, argP.varKeyPoint1_2, argP.varKeyPoint2_3);
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID3, argP.varID4, argP.varKeyPoint3_4, argP.varKeyPoint4_5);
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID5, argP.varID6, argP.varKeyPoint5_6, argP.varKeyPoint6_7);

				if constexpr (S) {
					/*添加尺寸约束*/
					AcDbObjectId varTmp;
					const auto varCPosR = mid(argP.varKeyPoint1, argP.varKeyPoint5);
					{
						AcGePoint3d varCPos = varCPosR;
						varCPos.y = argP.varKeyPoint1.y + std::signbit(argP.varKeyPoint1.y - argP.varKeyPoint5.y) ? -3.5 : 3.5;
						AcDbAssoc2dConstraintAPI::createHorizontalDimConstraint(argP.varID0, argP.varID3,
							argP.varKeyPoint0_1, argP.varKeyPoint3_4, varCPos, varTmp);
					}
					{
						AcGePoint3d varCPos = varCPosR;
						varCPos.x = argP.varKeyPoint1.x + std::signbit(argP.varKeyPoint1.x - argP.varKeyPoint5.x) ? -3.5 : 3.5;
						AcDbAssoc2dConstraintAPI::createVerticalDimConstraint(argP.varID1, argP.varID6,
							argP.varKeyPoint1_2, argP.varKeyPoint6_7, varCPos, varTmp);
					}
				}
			}

		}
	}/*namespace*/

	template<bool S >
	void _p_FRect_main(_p000_::PrivatePack&varData) try {
		using namespace _p000_;
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

		if constexpr(S) {
			/*select point*/
			if (false == _p_select_point(&varData)) {
				return;
			}
			/*get width*/
			_p_get_y(&varData);
			/*get height*/
			_p_get_x(&varData);
		}

		DrawARectPack varPackD;
		_p_draw_a_rect(varData.$Rects, varPackD, varData);
		_p_constraint<S>(varPackD);
	}
	catch (...) {
		return;
	}

	void FRect::main() try {
		using namespace _p000_;

		PrivatePack varData;

		/*construc pack*/
		if (false == _p_construct_pack(&varData)) {
			return;
		}

		_p_FRect_main<true>(varData);

	}
	catch (...) {
		return;
	}

	void  FRect::ftrect_main(
		double argx/*x*/,
		double argy/*y*/,
		double argw/*w*/,
		double argh/*h*/) try {
		using namespace _p000_;

		PrivatePack varData;

		/*construc pack*/
		if (false == _p_construct_pack(&varData)) {
			return;
		}

		varData.$Rects.$Height = argh;
		varData.$Rects.$Width = argw;
		varData.$StartPoint.x = argx;
		varData.$StartPoint.y = argy;

		_p_FRect_main<false>(varData);

	}
	catch (...) {
		return;
	}

}/*namespace sstd*/








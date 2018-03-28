#include "EFillet.hpp"
#include <cmath>
#include <array>
#include <acedCmdNF.h>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
}

namespace {
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {

	EFillet::EFillet() {
	}

	extern void loadEFillet() {
		EFillet::load();
	}

	void EFillet::load() {
		arx_add_main_command<EFillet>();
	}

	/****************/
	namespace {
		namespace __get_three_points {

			template<typename T>
			inline auto evalAxByC(T A, T x, T B, T y, T c) {
				return std::fma(A, x, std::fma(B, y, c));
			}

			template<typename T>
			class Line {
			public:
				T A, B, C;
				const T & m0;
				const T & n0;
				const T & m1;
				const T & n1;

				inline Line(const T & am0, const T &  an0, const T &  am1, const T &  an1) :
					m0(am0), n0(an0),
					m1(am1), n1(an1) {
					_p_eval();
				}
				T eval(const T & x, const T & y) {
					return evalAxByC(A, x, B, y, C);
				}

			private:
				inline void _p_eval() {
					A = n0 - n1;
					B = m1 - m0;
					C = std::fma(m0, n1, -m1 * n0);
					if (std::abs(A) > 1) {/*增加计算精度*/
						const auto tmp = T{ 1 } / A;
						A = T{ 1 };
						B *= tmp;
						C *= tmp;
					}
					if (std::abs(B) > 1) {/*增加计算精度*/
						const auto tmp = T{ 1 } / B;
						A *= tmp;
						B = T{ 1 };
						C *= tmp;
					}
				}
			public:
				/*
				K = (Ax+By+C)/(A*A+B*B)
				x = x - 2*A*K;
				y = y - 2*B*K;
				/*/
				inline std::array<T, 2> mirror(const T & x, const T& y) {
					auto K = std::fma(A, x, std::fma(B, y, C));
					if (K == 0) {
						return { x,y };
					}
					K /= std::fma(A, A, B*B);
					K *= T{ -2 };
					return { std::fma(K,A,x),std::fma(K,B,y) };
				}

			}/*Line*/;

			void getALine(double *P, AcDbLine*&L) {
				class Lock {
				public:
					ads_name varE;
					~Lock() { acedSSFree(varE); }
				}ss;

				if (RTNORM == acedEntSel(LR"(选择一条直线)", ss.varE, P)) {
					AcDbObjectId eId;
					acdbGetObjectId(eId, ss.varE);   //获取实体id  
					AcDbEntity * pEnt;
					if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForRead)) {
						throw 222;//打开实体失败，返回  
					}
					if (pEnt->isKindOf(AcDbLine::desc())) {
						L = AcDbLine::cast(pEnt);
					}
					else {
						throw 333;
					}
				}
				else { throw 111; }
			}

			bool get_three_point(
				AcGePoint3d&argStart,
				AcGePoint3d&argCenter,
				AcGePoint3d&argEnd,
				AcDbLine * & varFirstLine,
				AcDbLine * & varSecondLine
			) try {
				AcGePoint3d varPointFistLine;
				AcGePoint3d varPointSecondLine;
				AcGePoint3d varPointIntersect;

				{
					/*获得直线1******************/
					getALine(&varPointFistLine.x, varFirstLine);
					varPointFistLine = UcsToWorld(varPointFistLine);
				}

				{
					/*获得直线2******************/
					getALine(&varPointSecondLine.x, varSecondLine);
					varPointSecondLine = UcsToWorld(varPointSecondLine);
				}

				{
					/*计算两条直线的交点*********/
					AcGePoint3dArray varAns;
					varSecondLine->intersectWith(varFirstLine,
						AcDb::Intersect::kExtendBoth,
						varAns);
					if (varAns.isEmpty()) { throw 555; }
					varPointIntersect = varAns.first();
				}

				argCenter = varPointIntersect;

				{
					const auto varSP = varFirstLine->startPoint();
					const auto varEP = varFirstLine->endPoint();
					Line<double> varLine1x{ varSP.x,varSP.y,varEP.x,varEP.y };
					const auto varPointPP = varLine1x.mirror(varPointFistLine.x, varPointFistLine.y);
					varPointFistLine.x = std::fma(varPointFistLine.x, 0.5, 0.5*varPointPP[0]);
					varPointFistLine.y = std::fma(varPointFistLine.y, 0.5, 0.5*varPointPP[1]);
				}

				{
					const auto varSP = varSecondLine->startPoint();
					const auto varEP = varSecondLine->endPoint();
					Line<double> varLine1x{ varSP.x,varSP.y,varEP.x,varEP.y };
					const auto varPointPP = varLine1x.mirror(varPointSecondLine.x, varPointSecondLine.y);
					varPointSecondLine.x = std::fma(varPointSecondLine.x, 0.5, 0.5*varPointPP[0]);
					varPointSecondLine.y = std::fma(varPointSecondLine.y, 0.5, 0.5*varPointPP[1]);
				}

				const auto dx1 = varPointFistLine.x - varPointIntersect.x;
				const auto dy1 = varPointFistLine.y - varPointIntersect.y;

				const auto dx2 = varPointSecondLine.x - varPointIntersect.x;
				const auto dy2 = varPointSecondLine.y - varPointIntersect.y;

				if (std::signbit(dx1*dy2 - dy1 * dx2)) {
					argStart = varPointSecondLine;
					argEnd = varPointFistLine;
					std::swap(varFirstLine, varSecondLine);
				}
				else {
					argEnd = varPointSecondLine;
					argStart = varPointFistLine;
				}

				return true;
			}
			catch (...) { return false; }
		}
	}/*namespace*/

	void EFillet::main() try {
		AcDbObjectId ol1, ol2, ol3;
		AcGePoint3d varCenter;
		AcGePoint3d P1;
		AcGePoint3d P0;
		AcGePoint3d R0;

		{
			sstd::ArxClosePointer<AcDbLine> L0;
			sstd::ArxClosePointer<AcDbLine> L1;

			{/*0*/
				if (false == __get_three_points::get_three_point(P0, varCenter, P1, L0, L1)) {
					return;
				}

				ol1 = L0->objectId();
				ol2 = L1->objectId();

				double varLength = 0;
				if (RTNORM != acedGetDist(nullptr, LR"(输入圆角半径：)", &varLength)) { return; }
				if (varLength <= 0) { return; }

				auto dx1 = P0.x - varCenter.x;
				auto dy1 = P0.y - varCenter.y;

				auto dx2 = P1.x - varCenter.x;
				auto dy2 = P1.y - varCenter.y;

				{
					const auto l1 = std::hypot(dx1, dy1);
					if (l1 <= 0) { return; }
					const auto l2 = std::hypot(dx2, dy2);
					if (l2 <= 0) { return; }

					dx1 /= l1; dy1 /= l1;
					dx2 /= l2; dy2 /= l2;
				}

				const auto cons2a = dx1 * dx2 + dy1 * dy2;
				const auto sina = std::sqrt(std::abs(1 - cons2a))*0.5;

				if (sina == 0) { return; }

				dx1 += dx2;
				dy1 += dy2;

				{
					auto l = std::hypot(dx1, dy1);
					if (l == 0) { return; }
					l = 1.0 / l;
					dx1 *= l; dy1 *= l;
					l = varLength / sina;
					dx1 *= l;
					dy1 *= l;
				}

				R0.x = varCenter.x + dx1;
				R0.y = varCenter.y + dy1;

				{
					auto varDB = acdbHostApplicationServices()
						->workingDatabase();

					{
						sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
						sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

						{
							auto varE = varDB->getBlockTable(varBlockTable, AcDb::kForRead);
							if (varE != eOk) { throw varE; }
							varE = varBlockTable->getAt(ACDB_MODEL_SPACE,
								varBlockTableRecord,
								AcDb::kForWrite);
							if (varE != eOk) { throw varE; }
						}

						sstd::ArxClosePointer< AcDbArc > varLineFinal{ new  AcDbArc{
							 R0,
							 2.*varLength,
							 std::atan2(P1.y - R0.y,P1.x - R0.x),
							 std::atan2(P0.y - R0.y,P0.x - R0.x)
						} };
						varBlockTableRecord->appendAcDbEntity(varLineFinal);

						ol3 = varLineFinal->objectId();
						varLineFinal->setLayer(LR"(粗实线)");

					}

				}

				/*0*/}


			{
				class Lock {
				public:
					ads_name ss = {};
					Lock() { acedSSAdd(nullptr, nullptr, ss); }
					~Lock() { acedSSFree(ss); }
				} varLock;

				{
					ads_name eName;
					acdbGetAdsName(eName, ol1);
					acedSSAdd(eName, varLock.ss, varLock.ss);
				}

				{
					ads_name eName;
					acdbGetAdsName(eName, ol2);
					acedSSAdd(eName, varLock.ss, varLock.ss);
				}

				{
					ads_name eName;
					acdbGetAdsName(eName, ol3);
					acedSSAdd(eName, varLock.ss, varLock.ss);
				}

				acedCommandS(RTSTR, L"_.AutoConstrain",
					RTPICKS, varLock.ss,
					RTSTR, L"", RTNONE);

			}

		}

	}
	catch (...) {
		return;
	}

}/*namespace sstd*/



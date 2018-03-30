#include "ArxZCC.hpp"
#include <string_view>
#include <cmath>
#include <set>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"

using std::wstring_view;
using namespace std::string_view_literals;

namespace sstd {
	extern void UCS2WCS(const double *, double *);
}

namespace {
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {

	ArxZCC::ArxZCC() {

	}

	namespace {

		inline double getRandX() {
			constexpr const int Y0 = 300;
			constexpr const int Y1 = Y0 - (Y0 / 3);
			constexpr const double Y = Y0;
			double varX = std::rand() % (Y1);
			varX += (Y0 - Y1);
			return (std::rand() & 1) ? (varX / (Y + 10)) : ((-varX) / (Y + 10));
		}

		inline  AcGePoint3d addx(const AcGePoint3d& arg, const double x) {
			return{ arg.x + x,arg.y,0.0 };
		}

		inline  AcGePoint3d addy(const AcGePoint3d & arg, const double y) {
			return{ arg.x ,y + arg.y,0.0 };
		}

		inline sstd::ArxClosePointer< AcDbCircle > drawACirCle(
			const double & argR,
			const  AcGePoint3d & varPoint
		) {
			return new AcDbCircle(varPoint, AcGeVector3d{ 0,0,1 }, argR);
		}

		inline void getAPoint(
			const wstring_view & P,
			AcGePoint3d & varAns ) {
			auto $Error = acedGetPoint(nullptr, P.data(), &(varAns.x));
			if (RTNONE == $Error) { throw $Error; }
			else { if ($Error == RTNORM)return; }
			throw $Error;
		}

	}/*namespace*/

	void ArxZCC::main()try {
		AcGePoint3d varCP;
		getAPoint(LR"(获得圆心)"sv, varCP);
		const auto varCPWord = UcsToWorld(varCP);
		bool varRun = true;

		class RIndex : public AcDbObjectId {
		public:
			double $R = 0;
			RIndex(const  AcDbObjectId & i, const double & j) : AcDbObjectId(i), $R(j) {}
		};

		std::set<double> varRS;
		std::vector<RIndex> varIDV;
		AcDbObjectId LX1;
		AcDbObjectId LX2;
		AcDbObjectId LY1;
		AcDbObjectId LY2;

		auto varDB = acdbHostApplicationServices()
			->workingDatabase();

		{/*绘图*/
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

			while (varRun) try {

				AcGePoint3d varPointS;
				getAPoint(LR"(获得圆上一个点)"sv, varPointS);
				const auto varR = std::hypot(varCP.x - varPointS.x, varCP.y - varPointS.y);

				if (varR == 0) {
					continue;
				}

				if (varRS.count(varR) > 0) {
					continue;
				}

				varRS.insert(varR);

				{
					auto varDC = drawACirCle(varR, varCPWord);
					if (varDC) {
						varBlockTableRecord->appendAcDbEntity(varDC);
						varIDV.emplace_back(varDC->objectId(), varR);
						varDC->setLayer(LR"(粗实线)");
					}
					else {
						continue;
					}
				}

			}
			catch (...) { varRun = false; }/*while run*/

			if (varRS.empty()) { return; }
			const auto varRXX = *varRS.rbegin();
			{
				auto varDL = sstd::ArxClosePointer<AcDbLine>(
					new AcDbLine{ varCPWord,addx(varCPWord,varRXX) });
				varBlockTableRecord->appendAcDbEntity(varDL);
				varDL->setLayer(LR"(参考线)");
				LX1 = varDL->objectId();
			}
			{
				auto varDL = sstd::ArxClosePointer<AcDbLine>(
					new AcDbLine{ varCPWord,addx(varCPWord,-varRXX) });
				varBlockTableRecord->appendAcDbEntity(varDL);
				varDL->setLayer(LR"(参考线)");
				LX2 = varDL->objectId();
			}
			{
				auto varDL = sstd::ArxClosePointer<AcDbLine>(
					new AcDbLine{ varCPWord,addy(varCPWord,varRXX) });
				varBlockTableRecord->appendAcDbEntity(varDL);
				varDL->setLayer(LR"(参考线)");
				LY1 = varDL->objectId();
			}
			{
				auto varDL = sstd::ArxClosePointer<AcDbLine>(
					new AcDbLine{ varCPWord,addy(varCPWord,-varRXX) });
				varBlockTableRecord->appendAcDbEntity(varDL);
				varDL->setLayer(LR"(参考线)");
				LY2 = varDL->objectId();
			}

			/*绘图*/}

		{/*追加约束*/
			if (varIDV.size() > 1) {/*同心约束*/
				auto varID2I = varIDV.begin();
				auto varID1I = varID2I++;
				const auto varIDE = varIDV.end();
				for (; varID2I != varIDE; varID1I = varID2I++) {
					AcDbAssoc2dConstraintAPI::createConcentricConstraint(
						*varID2I, *varID1I, varCPWord, varCPWord);
				}
			}

			{/*水平垂直约束*/
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(LY2, varCPWord);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(LY1, varCPWord);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(LX1, varCPWord);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(LX2, varCPWord);
			}

			{/*重合约束*/
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(LX1, LX1, varCPWord, varCPWord);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(LX1, LY2, varCPWord, varCPWord);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(LX1, LX2, varCPWord, varCPWord);
			}

			{/*直径约束*/
				AcDbObjectId varTmp;
				for (const auto & varI : varIDV) {
					AcDbAssoc2dConstraintAPI::createDiamDimConstraint(
						varI, addy(varCPWord, varI.$R),
						addy(addx(varCPWord, varI.$R*getRandX()), varI.$R*getRandX()),
						varTmp);
				}
			}

		}

	}
	catch (...) {
		return;
	}

	void ArxZCC::load() {
		arx_add_main_command<ArxZCC>();
	}

	extern void loadArxZCC() {
		ArxZCC::load();
	}

}/*namespace sstd*/

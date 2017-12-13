#include <object_arx_global.hpp>
#include <vector>
#include "DrawMultiCircles.hpp"
#include <array>
#include <optional>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"

namespace sstd {

	extern void loadDrawMultiCircles() {
		DrawMultiCircles::load();
	}

	DrawMultiCircles::DrawMultiCircles() {

	}

	void DrawMultiCircles::load() {
		arx_add_main_command<DrawMultiCircles>();
	}

	namespace {
		class ThisMain {
		public:
			AcDbDatabase * const $DB;
			inline ThisMain(AcDbDatabase * arg) :$DB(arg) {}
			/************************************************/
			double $PCD = 0;
			double $D = 0;
			int $N = 0;
			AcGePoint3d $C;
			int $Error;
			sstd::ArxClosePointer<AcDbBlockTable> $BlockTable;
			sstd::ArxClosePointer<AcDbBlockTableRecord> $BlockTableRecord;
			AcDbObjectId $CurrentID;
			std::optional<AcDbObjectId> $DLintLayerID;
			std::optional<AcDbObjectId> $CLintLayerID;
			std::optional<AcDbObjectId> $RLintLayerID;

			class CircleItem {
			public:
				AcGePoint3d $CPoint;
				AcGePoint3d $CKPoint;
				AcGeVector3d $CVector;
				AcDbObjectId $CRLine;
				AcDbObjectId $CCirle;
				AcDbObjectId $CCLine;
				AcGePoint3d $KPoints[3];
			};
			AcDbObjectId $PCDCircle;
			std::vector<CircleItem> $CircleItems;

		public:
			inline void update_constraint();
			inline void update_this();
			inline void update_layer();
			inline void check_this();
			inline void draw_items();
			inline void check_error(int varG = eOk);
			inline void add(AcDbEntity*);
		};

		inline void ThisMain::update_layer() {
			sstd::ArxClosePointer< AcDbLayerTable > varLayer;
			$Error = $DB->getLayerTable(varLayer.pointer());
			check_error();
			AcDbObjectId varTID;
			if (eOk == varLayer->getAt(LR"(参考线)", varTID)) {
				$RLintLayerID = varTID;
			}
			if (eOk == varLayer->getAt(LR"(中心线)", varTID)) {
				$CLintLayerID = varTID;
			}
			if (eOk == varLayer->getAt(LR"(粗实线)", varTID)) {
				$DLintLayerID = varTID;
			}
		}

		inline void ThisMain::add(AcDbEntity*arg) {
			$BlockTableRecord->appendAcDbEntity($CurrentID, arg);
		}

		inline void ThisMain::check_error(int varG) {
			if ($Error == varG) { return; }
			throw $Error;
		}

		void ThisMain::check_this() {
			if ($N < 3) { throw 2; }
		}

		inline void ThisMain::update_this() {
			$Error = $DB->getBlockTable($BlockTable, AcDb::kForRead);
			check_error();
			$Error = $BlockTable->getAt(ACDB_MODEL_SPACE, $BlockTableRecord,
				AcDb::kForWrite);
			check_error();
			$Error = acedGetReal(LR"(请输入中心距<114.3>：)", &$PCD);
			if (RTNONE == $Error) { $PCD = 114.3; }
			else { check_error(RTNORM); }
			$Error = acedGetReal(LR"(请输入圆直径<15.3>：)", &$D);
			if (RTNONE == $Error) { $D = 15.3; }
			else { check_error(RTNORM); }
			$Error = acedGetInt(LR"(请输入圆的数量<5>：)", &$N);
			if (RTNONE == $Error) { $N = 5; }
			else { check_error(RTNORM); }
			$Error = acedGetPoint(nullptr, LR"(请输入圆的中心<0,0>：)", &($C.x));
			if (RTNONE == $Error) { $C = { 0.0,0.0,0.0 }; }
			else { check_error(RTNORM); }
		}

		template<typename T, typename U>
		inline void setLayer(T&a, const U&b) {
			if (b) { a->setLayer(*b, true, true); }
		}

		inline void ThisMain::draw_items() {
			$CircleItems.resize($N);

			const auto varStep = sstd::dpi() / $N;
			const auto varHPCD = $PCD*0.5;
			const auto varHD = $D*0.5;
			const AcGeVector3d varV = { 0.,0.,1. };
			const auto varStartAngle = sstd::hpi();
			/*绘制中心线圆*/
			{
				sstd::ArxClosePointer<AcDbCircle> varPCDCircle = new AcDbCircle($C,
					varV, varHPCD);
				this->add(varPCDCircle);
				setLayer(varPCDCircle, $CLintLayerID);
				$PCDCircle = varPCDCircle->objectId();
			}
			/*绘制中心孔*/
			for (double varI = 0; varI < $N; ++varI) {
				AcGePoint3d & varPos = $CircleItems[varI].$CPoint;
				varPos = $C;
				const auto varA = std::fma(varI,
					varStep,
					varStartAngle);
				AcGeVector3d & varXN = $CircleItems[varI].$CVector;
				varXN = AcGeVector3d(std::cos(varA), std::sin(varA), 0.);
				varPos += AcGeVector3d{
					varHPCD*varXN.x,
					varHPCD*varXN.y,
					0. };
				$CircleItems[varI].$CKPoint = varPos + AcGeVector3d{
					varHD*varXN.x,
					varHD*varXN.y,
					0. };
				sstd::ArxClosePointer<AcDbCircle> varCircle = new AcDbCircle(varPos,
					varV, varHD);
				this->add(varCircle);
				setLayer(varCircle, $DLintLayerID);
				$CircleItems[varI].$CCirle = varCircle->objectId();
			}
			/*绘制辅助线*/
			{
				//int varJ = 1;
				for (auto & varI : $CircleItems) {
					{// $KPoints
						auto & varK0 = varI.$KPoints[0];
						varK0 = $C + ((varI.$CPoint - $C)*1.3 +
							varHD *varI.$CVector);
						sstd::ArxClosePointer<AcDbLine> varLine =
							new AcDbLine($C, varK0);
						this->add(varLine);
						setLayer(varLine, $RLintLayerID);
						varI.$CRLine = varLine->objectId();
					}
					/*{
						sstd::ArxClosePointer<AcDbLine> varLine =
							new AcDbLine(varI.$CPoint, $CircleItems[varJ].$CPoint);
						this->add(varLine);
						setLayer(varLine, $RLintLayerID);
						++varJ;
						if (varJ >= $N) { varJ = 0; }
					}*/
				}
			}
			{/*中心线*/
				const auto varHDAdd3_0 = varHD + 3.;
				for (double varI = 0; varI < $N; ++varI) {
					auto & varK1 = $CircleItems[varI].$KPoints[1];
					auto & varK2 = $CircleItems[varI].$KPoints[2];
					const auto & varC = $CircleItems[varI].$CPoint;
					const auto & varCN = $CircleItems[varI].$CVector;
					const auto varCN0 = varCN*varHDAdd3_0;
					varK1 = varC + varCN0;
					varK2 = varC - varCN0;
					sstd::ArxClosePointer<AcDbLine> varLine =
						new AcDbLine(varK1, varK2);
					this->add(varLine);
					setLayer(varLine, $CLintLayerID);
					$CircleItems[varI].$CCLine = varLine->objectId();
				}
			}

			$BlockTableRecord.close();
			$BlockTable.close();
		}

		static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
			AcGeMatrix3d ucs;
			acedGetCurrentUCS(ucs);
			AcGePoint3d res(ucsPoint);
			return res.transformBy(ucs);
		}

		inline void ThisMain::update_constraint() {
			AcDbObjectId varDimID;
			AcDbAssoc2dConstraintAPI::createVerticalConstraint(
				$CircleItems[0].$CRLine,
				$CircleItems[0].$CPoint);

			const auto varNSub1 = $N - 1;
			int varIndexLast = -1;
			int varIndexCurrent = 0;

			$C = UcsToWorld($C);
			for (auto & varI : $CircleItems) {
				auto & varK0 = varI.$KPoints[0];
				auto & varK1 = varI.$KPoints[1];
				auto & varK2 = varI.$KPoints[2];
				varI.$CPoint = UcsToWorld(varI.$CPoint);
				varI.$CKPoint = UcsToWorld(varI.$CKPoint);
				varK0 = UcsToWorld(varK0);
				varK1 = UcsToWorld(varK1);
				varK2 = UcsToWorld(varK2);
			}

			for (; varIndexCurrent < varNSub1; ) {
				varIndexLast = varIndexCurrent;
				++varIndexCurrent;
				auto & varCurrent = $CircleItems[varIndexCurrent];
				auto & varLast = $CircleItems[varIndexLast];
				//AcDbAssocVariable

				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(
					varCurrent.$CRLine, varLast.$CRLine, $C, $C);

				AcDbAssoc2dConstraintAPI::create2LineAngularDimConstraint(
					varCurrent.$CRLine, varLast.$CRLine,
					varCurrent.$CPoint, varLast.$CPoint,
					AcGePoint3d((varCurrent.$CPoint.x + varLast.$CPoint.x)*0.5,
					(varCurrent.$CPoint.y + varLast.$CPoint.y)*0.5, 0.),
					varDimID
				);

				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(
					varCurrent.$CRLine, varLast.$CRLine,
					$C, $C
				);

				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(
					varCurrent.$CCLine, varLast.$CCLine,
					varCurrent.$CPoint, varLast.$CPoint
				);

				AcDbAssoc2dConstraintAPI::createEqualRadiusConstraint(
					varCurrent.$CCirle, varLast.$CCirle,
					varCurrent.$CKPoint, varLast.$CKPoint
				);
			}

			for (const auto & varI : $CircleItems) {
				AcDbAssoc2dConstraintAPI::createColinearConstraint(
					varI.$CCLine, varI.$CRLine,
					varI.$CPoint, varI.$CPoint
				);

			}

			AcDbAssoc2dConstraintAPI::createDiamDimConstraint(
				$CircleItems[0].$CCirle,
				$CircleItems[0].$CPoint,
				$CircleItems[0].$CKPoint,
				varDimID
			);

			/*{
				constexpr auto K = 0 ;
				const auto & var = $CircleItems[K];
				AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
					var.$CRLine,
					var.$KPoints[0],
					$C,
					var.$KPoints[0] + var.$CVector,
					varDimID
				);
			}*/

			/*{
				constexpr auto K = 2;
				AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
					$CircleItems[K].$CCLine,
					$CircleItems[K].$KPoints[1],
					$CircleItems[K].$KPoints[2],
					$CircleItems[K].$KPoints[1] + $CircleItems[K].$CVector,
					varDimID
				);
			}*/

			AcDbAssoc2dConstraintAPI::createCoincidentConstraint(
				$PCDCircle, $CircleItems[0].$CRLine,
				$C, $C);

			AcDbAssoc2dConstraintAPI::createDiamDimConstraint(
				$PCDCircle,
				$C,
				$CircleItems[1].$CKPoint,
				varDimID
			);

		}

	}/*namespace*/

	void DrawMultiCircles::main() try {
		std::unique_ptr<ThisMain>thisMain{
			new ThisMain{ acdbHostApplicationServices()->workingDatabase() } };
		thisMain->update_layer();
		thisMain->update_this();
		thisMain->check_this();
		thisMain->draw_items();
		thisMain->update_constraint();

	}
	catch (...) { return; }

}/*namespace sstd*/


#include <object_arx_global.hpp>
#include <vector>
#include "DrawMultiCircles.hpp"
#include <array>
#include <optional>

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
		public:
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
			if (eOk == varLayer->getAt(LR"(参考线)",varTID)) {
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

		void ThisMain::check_this() {}

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

		template<typename T,typename U>
		inline void setLayer(T&a,const U&b) {
			if (b) { a->setLayer(*b, true, true); }
		}

		inline void ThisMain::draw_items() {
			std::vector<AcGePoint3d> varCPoints;
			std::vector<AcGeVector3d> varCNormal;
			varCNormal.reserve($N);
			varCPoints.reserve($N);
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
			}
			/*绘制中心孔*/
			for (double varI = 0; varI < $N; ++varI) {
				AcGePoint3d varPos = $C;
				const auto varA = std::fma(varI,
					varStep,
					varStartAngle);
				AcGeVector3d varXN(std::cos(varA), std::sin(varA), 0.);
				varCNormal.push_back(varXN);
				varPos += AcGeVector3d{
					varHPCD*varXN.x,
					varHPCD*varXN.y,
					0. };
				varCPoints.push_back(varPos);
				sstd::ArxClosePointer<AcDbCircle> varCircle = new AcDbCircle(varPos,
					varV, varHD);
				this->add(varCircle);
				setLayer(varCircle, $DLintLayerID);
			}
			/*绘制辅助线*/
			{
				for (const auto & varI : varCPoints) {
					sstd::ArxClosePointer<AcDbLine> varLine =
						new AcDbLine($C, $C + (varI - $C)*1.3);
					this->add(varLine);
					setLayer(varLine, $RLintLayerID);
				}
			}
			{
				const auto varHDAdd3_0 = varHD + 3.;
				for (double varI = 0; varI < $N; ++varI) {
					const auto & varC = varCPoints[varI];
					const auto & varCN = varCNormal[varI];
					const auto varCN0 = varCN*varHDAdd3_0;
					sstd::ArxClosePointer<AcDbLine> varLine =
						new AcDbLine(varC + varCN0, varC - varCN0);
					this->add(varLine);
					setLayer(varLine, $CLintLayerID);
				}
			}
		}

	}/*namespace*/

	void DrawMultiCircles::main() try {
		std::unique_ptr<ThisMain>thisMain{
			new ThisMain{ acdbHostApplicationServices()->workingDatabase() } };
		thisMain->update_layer();
		thisMain->update_this();
		thisMain->check_this();
		thisMain->draw_items();
	}
	catch (...) { return; }

}/*namespace sstd*/


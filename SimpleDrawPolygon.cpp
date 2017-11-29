#include "SimpleDrawPolygon.hpp"
#include <cmath>
#include <vector>

namespace sstd {

	extern void loadSimpleDrawPolygon() { SimpleDrawPolygon::load(); }

	SimpleDrawPolygon::SimpleDrawPolygon() {

	}

	void SimpleDrawPolygon::load() {
		arx_add_main_command<SimpleDrawPolygon>();
	}

	void SimpleDrawPolygon::main() {
		int varEdgesNumber = 0;
		double varFirstEdgeAngle = 0;

		auto varGetEdgetsNumbe = [&varEdgesNumber]()->bool {
			int varReturn = acedGetInt(LR"(请输入边数<3>：)", &varEdgesNumber);
			if (RTNONE == varReturn) {
				varEdgesNumber = 3;
				return true;
			}
			if ((RTNORM == varReturn) && (varEdgesNumber > 2)) {
				return true;
			}

			acutPrintf(LR"(您输入了一个无效值
)");
			return false;
		};

		auto varGetFirstEdgeAngle = [&varFirstEdgeAngle]()->bool {
			/*ANGBASE*/
			int varReturn = acedGetAngle(nullptr,LR"(请输入旋转角度<0>：)", &varFirstEdgeAngle);
			if (RTNONE == varReturn) {
				varFirstEdgeAngle = 0;
				return true;
			}
			if (RTNORM == varReturn) {
				return true;
			}

			acutPrintf(LR"(您输入了一个无效值
)");
			return false;
		};

		/*input the number of the edge*/
		if (false == varGetEdgetsNumbe()) {
			return;
		}
		if (false == varGetFirstEdgeAngle()) {
			return;
		}

		const double varAngleStep = sstd::dpi<double>() / varEdgesNumber;
		constexpr double varEdgeLength = 100;
		const AcGePoint3d varPointZero{ 0,0,0 };
		std::vector< sstd::ArxClosePointer<AcDbLine> > varLines;
		varLines.reserve(varEdgesNumber * 2);
		std::vector< AcGePoint3d > varEdgePoints;
		varEdgePoints.reserve(varEdgesNumber);

		for (int varI = 0; varI < varEdgesNumber; ++varI) {
			const auto varAngle = std::fma(varAngleStep, varI, varFirstEdgeAngle);
			varEdgePoints.emplace_back(
				varEdgeLength*std::cos(varAngle),
				varEdgeLength*std::sin(varAngle),
				0.0);
		}

		auto varPoint1 = varEdgePoints.begin();
		auto varPoint2 = 1 + varPoint1;
		auto const varEnd = varEdgePoints.end();
		for (; varPoint2 != varEnd; ++varPoint2) {
			varLines.emplace_back(new AcDbLine{ *varPoint1,*varPoint2 });
			varLines.emplace_back(new AcDbLine{ *varPoint1,varPointZero });
			varPoint1 = varPoint2;
		}
		varLines.emplace_back(new AcDbLine{ *varPoint1,varPointZero });
		varLines.emplace_back(new AcDbLine{ varEdgePoints[0],*varPoint1 });
		varEdgePoints.clear();

		sstd::addToModelSpace(varLines.begin(), varLines.end());
	}

}/*sstd*/



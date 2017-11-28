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
		constexpr int varEdgesNumber = 77;
		constexpr double varAngleStep = 3.141592654 * 2 / varEdgesNumber;
		constexpr double varEdgeLength = 100;
		const AcGePoint3d varPointZero{ 0,0,0 };
		std::vector< sstd::ArxClosePointer<AcDbLine> > varLines;
		varLines.reserve(varEdgesNumber * 2);
		std::vector< AcGePoint3d > varEdgePoints;
		varEdgePoints.reserve(varEdgesNumber);

		for (int varI = 0; varI < varEdgesNumber; ++varI) {
			const auto varAngle = varI*varAngleStep;
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



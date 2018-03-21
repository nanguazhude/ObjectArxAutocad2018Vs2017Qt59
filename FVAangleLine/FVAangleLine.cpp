
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
					StartPoint=0,
					StartPointHigh=1,
					StartPointLow=2,

					MiddlePoint = 3,
					MiddlePointHigh = 4,
					MiddlePointLow = 5,

					EndPoint = 6,
					EndPointHigh = 7,
					EndPointLow = 8,
				};

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

				inline void check_error(int );
			};

			inline void Main::check_error(int varG) {
				if ($Error == varG) { return; }
				throw $Error;
			}

			inline Main::Main() {
				$DB = acdbHostApplicationServices()->workingDatabase();
				$SLayerID= $DB->clayer();
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
				if (RTNONE == $Error) { $VDist2 = 15.3; }
				else { check_error(RTNORM); }
			}

			inline void Main::getHDist() {
				$Error = acedGetDist(nullptr, LR"(请输入水平距离<15.3>：)", &$HDist );
				if (RTNONE == $Error) { $HDist = 15.3; }
				else { check_error(RTNORM); }
			}

			inline void Main::getAngle() {
				$Error = acedGetReal(LR"(请输入角度<90>：)", &$Angle);
				if (RTNONE == $Error) { $Angle = 90; }
				else { check_error(RTNORM); }
			}

			inline void Main::getStartPoint(){
				$Error = acedGetPoint(nullptr, LR"(请输入起始中心<0,0>：)", &($StartPoint.x));
				if (RTNONE == $Error) { $StartPoint = { 0.0,0.0,0.0 }; }
				else { check_error( RTNORM); }
				$StartPoint = UcsToWorld($StartPoint);
			}

			inline void Main::exec() {

				$VDist1 = std::abs($VDist1);
				$VDist2 = std::abs($VDist2);
				if ($VDist2 == 0) { throw 0; }
				if ($VDist1 == 0) { throw 1; }
				if ($VDist1 == $VDist2 ) { throw 2; }

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
				constexpr const auto div_180 = 1.0 / 180.0;
				constexpr const auto K = div_180 * pi;

				const bool V1BV2 = $VDist1 > $VDist2;
				const auto VV1V2 = V1BV2 ? 
					($VDist1- $VDist2) * std::signbit($HDist) :
					($VDist2- $VDist1) ;

				/* 0-90 */
				if ($Angle==90) { throw 90; }
				const auto  HV1V2 = VV1V2 * std::tan(K*$Angle);

				if (V1BV2) {
					$Points[MiddlePoint] = $Points[StartPoint];
					$Points[MiddlePoint].x += HV1V2;
				}
				else {

				}

			}

			inline void Main::draw() {
			}

			inline void Main::constraint() {
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


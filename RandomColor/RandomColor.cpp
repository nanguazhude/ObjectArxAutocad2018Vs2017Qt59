/*https://www.cnblogs.com/wangyblzu/p/5710715.html*/
#include <cstddef>
#include <cstdlib>
#include <array>
#include "object_arx_global.hpp"
namespace sstd {
 
	extern std::array<std::uint8_t,3> randColr() {
		bool isHOk = false;
		bool isSOk = false;
		bool isVOk = false;

		int H, S, V;

		/*0 to 359*/
		while (false == isHOk) {
			H = std::rand() % 359;
			if ((H>=150)&&(H<=210)) {
				continue;
			}
			isHOk = true;
		}
		//acutPrintf(LR"(A)");
		/*0 to 255*/
		while (false==isSOk) {
			S = std::rand() & 255;
			if ((S>125)&&(S<215)) {
				isSOk = true;
			}
		}
		//acutPrintf(LR"(B)");
		/*0 to 255*/
		while (false==isVOk) {
			V = std::rand() & 255;
			if (V > 178) {
				isVOk = true;
			}
		}
		//acutPrintf(LR"(C)");
		QColor varAns;
		varAns.setHsv( H,S,V );
		int r, g, b;
		varAns.getRgb(&r,&g,&b);

		std::array<std::uint8_t, 3> varAnsP;
		varAnsP[0] = static_cast<std::uint8_t>(r&0x00ff);
		varAnsP[1] = static_cast<std::uint8_t>(g&0x00ff);
		varAnsP[2] = static_cast<std::uint8_t>(b&0x00ff);

		return varAnsP;
	}

}
//////////////////












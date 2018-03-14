#include <cmath>
namespace sstd::_private_FARC_ALG_HPP {
	namespace {
		/*return A * x + B * y + C */
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
		};

		template<typename T>
		bool eval(const T argM0,
			const T argN0,
			const T argM1,
			const T argN1,
			const T x0, const T y0,
			T sx0, T sy0,
			const T r,
			T * ans/*cx,cy x0,y0 x1,y1 */) {
			if (ans == nullptr) { return true; }
			//求点0到直线0的距离
			Line<T> _L0{ argM0,argN0,argM1,argN1 };
			if ((_L0.A == 0) && (0 == _L0.B)) {
				return false;
			}
			const auto varE_X0_Y0_Line0 = _L0.eval(x0, y0);
			if (varE_X0_Y0_Line0 == 0) {
				return false;
			}
			const auto sqrt_A_B_0 = std::hypot(_L0.A, _L0.B);
			const T var_tmp01 = T{ 1 } / sqrt_A_B_0;
			auto var_tmp0 = var_tmp01;
			const auto _PN_Length_P0_L0 = varE_X0_Y0_Line0 * var_tmp0;
			const auto Length_P0_L0 = std::abs(_PN_Length_P0_L0);
			//求点0关于直线0的对称点
			var_tmp0 *= T{ -2 };
			const auto x0_P0 = std::fma(var_tmp0, _L0.A*_PN_Length_P0_L0, x0);
			const auto y0_P0 = std::fma(var_tmp0, _L0.B*_PN_Length_P0_L0, y0);
			//求点0到圆心在直线0上的投影的长度
			const auto r_r = r * r;
			var_tmp0 = Length_P0_L0 - r;
			var_tmp0 = r_r - var_tmp0 * var_tmp0;
			if (var_tmp0 < 0) {
				return false;
			}
			const auto LDx = std::sqrt(var_tmp0);
			//求出点0在直线0上的垂足
			const auto Px0 = (x0_P0 + x0)*T { 0.5 };
			const auto Py0 = (y0_P0 + y0)*T { 0.5 };
			//求出s0在直线0上的垂足
			do {
				const auto varE_X0_Y0_Line0 = _L0.eval(sx0, sy0);
				if (varE_X0_Y0_Line0 == 0) { break; }
				auto var_tmp0 = var_tmp01;
				const auto _PN_Length_P0_L0 = varE_X0_Y0_Line0 * var_tmp0;
				var_tmp0 *= T{ -2 };
				const auto sx10 = std::fma(var_tmp0, _L0.A*_PN_Length_P0_L0, sx0);
				const auto sy10 = std::fma(var_tmp0, _L0.B*_PN_Length_P0_L0, sy0);
				sx0 = (sx10 + sx0)*T { 0.5 };
				sy0 = (sy10 + sy0)*T { 0.5 };
			} while (false);
			if ((Px0 == sx0) && (Py0 == sy0)) { return false; }
			//求出圆心在直线0上的垂足
			auto Dxx = (sx0 - Px0);
			auto Dxy = (sy0 - Py0);
			if ((Dxx == 0) && (Dxy == 0)) {
				return false;
			}
			{
				var_tmp0 = std::hypot(Dxx, Dxy);
				var_tmp0 = T{ 1 } / var_tmp0;
				Dxx *= var_tmp0;
				Dxy *= var_tmp0;
			}
			const auto x1 = std::fma(Dxx, LDx, Px0);
			const auto y1 = std::fma(Dxy, LDx, Py0);
			/*求出圆心*/
			auto Dyx = x0 - x0_P0;
			auto Dyy = y0 - y0_P0;
			if ((Dyx == 0) && (Dyy == 0)) {
				return false;
			}
			{
				var_tmp0 = std::hypot(Dyx, Dyy);
				var_tmp0 = T{ 1 } / var_tmp0;
				Dyx *= var_tmp0;
				Dyy *= var_tmp0;
			}
			const auto Dcx = std::fma(Dyx, r, x1);
			const auto Dcy = std::fma(Dyy, r, y1);

			*ans++ = Dcx;
			*ans++ = Dcy;
			if (((Dxx*Dyy) - (Dxy*Dyx)) < 0) {
				*ans++ = x1;
				*ans++ = y1;
				*ans++ = x0;
				*ans++ = y0;
			}
			else {
				*ans++ = x0;
				*ans++ = y0;
				*ans++ = x1;
				*ans++ = y1;
			}

			return true;
		}
	}
}/*sstd*/

/**

#include "FARC_ALG.hpp"
#include <iostream>

int main(int ,char **) {

double ans[6];
sstd::_private_FARC_ALG_HPP::eval<double>(
100,133,
277,394,
255,456,
1+100, 2+133,
120,
ans
);

for (const auto & i:ans) {
std::cout << i << std::endl;
}


system("pause");
}

143.675
411.203
242.991
343.851
255
456

**/



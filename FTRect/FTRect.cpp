#include "FTRect.hpp"
#include "../FRect/FRect.hpp"

namespace sstd {

	FTRect::FTRect() {
	}

	void FTRect::load() {
		arx_add_main_command<FTRect>();
	}

	inline void check_error(const int & arg, const int & varG) {
		if (arg == varG) { return; }
		svthrow(LR"(断言失败)"sv);
	}

	template<bool S>
	inline void _p_FTRect_main() {
		double xywh[4];

		{
			auto $Error = acedGetPoint(nullptr, LR"(请输入起点<0,0>：)", xywh);
			if (RTNONE == $Error) { xywh[0] = 0; xywh[1] = 0; }
			check_error($Error, RTNORM);
		}

		{
			double xywh1[4];
			auto $Error = acedGetPoint(nullptr, LR"(请输入终点<0,0>：)", xywh1);
			if (RTNONE == $Error) { xywh1[0] = 0; xywh1[1] = 0; }
			check_error($Error, RTNORM);

			if constexpr(S) {
				xywh[2] = xywh[0] - xywh1[0];
				xywh[3] = xywh1[1] - xywh[1];
				xywh[1] = (xywh[1] + xywh1[1])*0.5;
			}
			else {
				xywh[2] = xywh[0] - xywh1[0];
				xywh[3] = 2*(xywh1[1] - xywh[1]);
			}

		}

		sstd::FRect::ftrect_main(xywh[0], xywh[1], xywh[2], xywh[3]);
	}

	void FTRect::main() try {
		_p_FTRect_main<true>();
	}
	catch (...) {
		return;
	}

	extern void loadFTRect() {
		FTRect::load();
	}

	namespace {
		class FCTRect {
		public:
			FCTRect()=default;
			static void load() {
				arx_add_main_command<FCTRect>();
			}
			static void main() try{
				_p_FTRect_main<false>();
			}
			catch (...) { return; }
			DEFINE_ARX_NAME(LR"(_fctrect)")
		};
	}

	extern void loadFCTRect() {
		FCTRect::load();
	}

}



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
		throw arg;
	}

	void FTRect::main() try {

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

			xywh[2] = xywh[0] - xywh1[0];
			xywh[3] = xywh1[1] - xywh[1];

			xywh[1] = (xywh[1] + xywh1[1])*0.5;
		}

		sstd::FRect::ftrect_main(xywh[0], xywh[1], xywh[2], xywh[3]);
	}
	catch (...) {
		return;
	}

	extern void loadFTRect() {
		FTRect::load();
	}

}



#ifndef DRAWMSTEPS_HPP
#define DRAWMSTEPS_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class DrawMSteps{
	public:
		DrawMSteps();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_drawmsteps)")
	};

}

#endif // DRAWMSTEPS_HPP

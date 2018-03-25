#ifndef FRECT_HPP
#define FRECT_HPP

#include <object_arx_global.hpp>

namespace sstd {
	class FRect {
	public:
		FRect();

		static void load();
		static void main();
		static void ftrect_main(double/*x*/, double/*y*/, double/*w*/, double/*h*/);
		DEFINE_ARX_NAME(LR"(_frect)")
	};
}/*namespace*/

#endif // FRECT_HPP

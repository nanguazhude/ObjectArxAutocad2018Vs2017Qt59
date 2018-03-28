#ifndef MAKE_STYLE_DIM_HPP
#define MAKE_STYLE_DIM_HPP
#include <object_arx_global.hpp>
namespace sstd {
	class MakeDimStyle {
	public:
		MakeDimStyle();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_makedimstyle)")
	};
}/**************/

#endif

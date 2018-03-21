#ifndef HPP_FVANGLE_LINE_HPP
#define HPP_FVANGLE_LINE_HPP

#include <object_arx_global.hpp>

namespace sstd {
	class FVAngleLine {
	public:
		FVAngleLine()=default;

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_fvangleline)")
	};
}/*namespace sstd*/


#endif



#ifndef SIMPLEDRAWPOLYGON_HPP
#define SIMPLEDRAWPOLYGON_HPP

#include "object_arx_global.hpp"

namespace sstd {

	class SimpleDrawPolygon {
	public:
		SimpleDrawPolygon();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_simpledrawpolygon)")
	};

}/*namespace sstd*/

#endif // SIMPLEDRAWPOLYGON_HPP

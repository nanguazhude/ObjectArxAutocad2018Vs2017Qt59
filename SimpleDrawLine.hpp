#ifndef SIMPLEDRAWLINE_HPP
#define SIMPLEDRAWLINE_HPP

#include "object_arx_global.hpp"

namespace sstd {

	class SimpleDrawLine {
	public:
		SimpleDrawLine();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_simpledrawline)")
	};

}/*namespace sstd*/

#endif // SIMPLEDRAWLINE_HPP

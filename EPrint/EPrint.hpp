#ifndef EPRINT_EFILLET_HPP
#define EPRINT_EFILLET_HPP

#include <object_arx_global.hpp>

namespace sstd {
	class EPrint {
	public:
		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_eprint)")
	};
}/*namespace sstd*/

#endif


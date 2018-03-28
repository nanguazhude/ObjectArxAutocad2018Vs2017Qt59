#ifndef EFILLET_HPP
#define EFILLET_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class EFillet {
	public:
		EFillet();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_efillet)")
	};

}/*namespace sstd*/

#endif // EFILLET_HPP

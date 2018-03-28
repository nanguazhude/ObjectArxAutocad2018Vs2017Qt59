#ifndef ECHAMFER_HPP
#define ECHAMFER_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class EChamfer {
	public:
		EChamfer();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_echamfer)")
	};

}/*namespace sstd*/

#endif // ECHAMFER_HPP


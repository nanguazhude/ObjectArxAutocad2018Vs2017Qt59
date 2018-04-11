#ifndef FCONNECT_HPP
#define FCONNECT_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class FConnect {
	public:
		FConnect();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_fconnect)")
	};

}/*namespace sstd*/

#endif // FCONNECT_HPP

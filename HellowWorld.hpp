#ifndef HELLOWWORLD_H
#define HELLOWWORLD_H

#include "object_arx_global.hpp"

namespace sstd {

	class HellowWorld {

	public:
		HellowWorld();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_hellowworld)")
	};

}/*namespace sstd*/

#endif // HellowWorld_H





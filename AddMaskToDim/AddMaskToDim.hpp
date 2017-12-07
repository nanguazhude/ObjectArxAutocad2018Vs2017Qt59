#ifndef AddMaskToDim_HPP
#define AddMaskToDim_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class AddMaskToDim {
	public:
		AddMaskToDim();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_AddMaskToDim)")
	};

}/*namespace sstd*/

#endif // AddMaskToDim_HPP

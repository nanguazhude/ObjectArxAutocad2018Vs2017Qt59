#ifndef UPDATEDIMSTYLE_HPP
#define UPDATEDIMSTYLE_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class UpdateDimStyle {
	public:
		UpdateDimStyle();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_updatedimstyle)")
	};

}/*namespace sstd*/

#endif // UPDATEDIMSTYLE_HPP

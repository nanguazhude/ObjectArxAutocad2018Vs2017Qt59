#ifndef UPDATEDIMSTYLE_HPP
#define UPDATEDIMSTYLE_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class UpdateDimStyle {
	public:
		UpdateDimStyle();

		static void load();
		static void main();
		static void main_do_not_change_text_height();
		DEFINE_ARX_NAME(LR"(_updatedimstyle)")
	};

}/*namespace sstd*/

#endif // UPDATEDIMSTYLE_HPP

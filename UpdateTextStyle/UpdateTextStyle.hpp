#ifndef UPDATETEXTSTYLE_HPP
#define UPDATETEXTSTYLE_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class UpdateTextStyle {
	public:
		UpdateTextStyle();

		static void load();
		static void main();
		static void main_do_not_change_text_height();
		DEFINE_ARX_NAME(LR"(_updatetexttyle)")
	};

}/*namespace sstd*/

#endif // UPDATETEXTSTYLE_HPP

#ifndef UPDATELAYER_HPP
#define UPDATELAYER_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class UpdateLayer {
	public:
		UpdateLayer();

		static void load();
		static void main();
		static void main_do_not_change_text_height();
		DEFINE_ARX_NAME(LR"(_updatelayer)")
	};

}/*namespace sstd*/

#endif // UPDATELAYER_HPP

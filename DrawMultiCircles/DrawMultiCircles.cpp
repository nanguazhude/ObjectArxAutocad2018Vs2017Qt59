#include "DrawMultiCircles.hpp"

namespace sstd {

	extern void loadDrawMultiCircles() {
		DrawMultiCircles::load();
	}

	DrawMultiCircles::DrawMultiCircles() {

	}

	void DrawMultiCircles::load() {
		arx_add_main_command<DrawMultiCircles>();
	}

	namespace {
		class ThisMain {
		public:
			AcDbDatabase * const $DB;
			inline ThisMain(AcDbDatabase * arg) :$DB(arg) {}
		};
	}/*namespace*/

	void DrawMultiCircles::main() {
		ThisMain thisMain{ acdbHostApplicationServices()->workingDatabase() };
	}

}/*namespace sstd*/


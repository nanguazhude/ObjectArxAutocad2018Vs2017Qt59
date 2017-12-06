#include <object_arx_global.hpp>
#include "UpdatePart.hpp"

/*****************************************************/
#include "../UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.hpp"
#include "../UpdateLayer/UpdateLayer.hpp"
#include "../UpdateTextStyle/UpdateTextStyle.hpp"
#include "../UpdateDimStyle/UpdateDimStyle.hpp"
#include "../UpdateMLeaderStyle/UpdateMLeaderStyle.hpp"
/*****************************************************/

sstd::UpdatePart::UpdatePart() {
}
namespace sstd {
	namespace {
		class ThisFunction {
			AcDbDatabase * const $DB;
		public:
			inline ThisFunction( AcDbDatabase * arg ):$DB(arg) {}
			inline void UpdatePart();
		};
		inline void ThisFunction::UpdatePart() {
			UpdateBlockFromOtherFile::main();
			UpdateLayer::main();
			UpdateTextStyle::main();
			UpdateDimStyle::main();
			UpdateMLeaderStyle::main();
			auto varCD = acDocManager->curDocument();
			if (varCD) {
				acDocManager->sendStringToExecute(
					varCD,
					LR"(vvvvvvattsyngriducsicon
)"
				);
			}
		}
	}/*namespace*/
}/*namespace sstd*/

void sstd::UpdatePart::load() {
	arx_add_main_command<UpdatePart>();
}

void sstd::UpdatePart::main() {
	ThisFunction varFunction( acdbHostApplicationServices()->workingDatabase() );
	varFunction.UpdatePart();
}

namespace sstd {
	extern void loadUpdatePart() { UpdatePart::load(); }
}/*namespace*/


#include <object_arx_global.hpp>
#include "UpdateAll.hpp"

/*****************************************************/
#include "../UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.hpp"
#include "../UpdateLayer/UpdateLayer.hpp"
#include "../UpdateTextStyle/UpdateTextStyle.hpp"
#include "../UpdateDimStyle/UpdateDimStyle.hpp"
#include "../UpdateMLeaderStyle/UpdateMLeaderStyle.hpp"
#include "../UpdateTitleBlockTime/UpdateTitleBlockTime.hpp"
#include "../CopyFilePathToClipboard/CopyFilePathToClipboard.hpp"
/*****************************************************/

sstd::UpdateAll::UpdateAll() {
}
namespace sstd {
	namespace {
		class ThisFunction {
			AcDbDatabase * const $DB;
		public:
			inline ThisFunction( AcDbDatabase * arg ):$DB(arg) {}
			inline void updateAll();
		};
		inline void ThisFunction::updateAll() {
			UpdateTextStyle::main();
			UpdateBlockFromOtherFile::main();
			UpdateDimStyle::main();
			UpdateMLeaderStyle::main();
			UpdateTitleBlockTime::main();
			UpdateLayer::main();
			CopyFilePathToClipboard::main();
			{
				auto DB = acdbHostApplicationServices()->workingDatabase();
				DB->setDimAssoc(2);
			}
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

void sstd::UpdateAll::load() {
	arx_add_main_command<UpdateAll>();
}

void sstd::UpdateAll::main() {
	ThisFunction varFunction( acdbHostApplicationServices()->workingDatabase() );
	varFunction.updateAll();
}

namespace sstd {
	extern void loadUpdateAll() { UpdateAll::load(); }
}/*namespace*/


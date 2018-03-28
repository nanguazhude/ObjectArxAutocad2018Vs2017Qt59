#include <object_arx_global.hpp>
#include "UpdatePart.hpp"

/*****************************************************/
#include "../UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.hpp"
#include "../UpdateLayer/UpdateLayer.hpp"
#include "../UpdateTextStyle/UpdateTextStyle.hpp"
#include "../UpdateDimStyle/UpdateDimStyle.hpp"
#include "../UpdateMLeaderStyle/UpdateMLeaderStyle.hpp"
#include "../UpdateTitleBlockTime/UpdateTitleBlockTime.hpp"
#include "../CopyFilePathToClipboard/CopyFilePathToClipboard.hpp"
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
			UpdateLayer::main_do_not_change_text_height();
			UpdateTextStyle::main_do_not_change_text_height();
			UpdateBlockFromOtherFile::main();
			UpdateDimStyle::main_do_not_change_text_height();
			UpdateMLeaderStyle::main_do_not_change_text_height();
			UpdateTitleBlockTime::main();
			CopyFilePathToClipboard::main();
			{
				auto DB = acdbHostApplicationServices()->workingDatabase();
				DB->setDimAssoc(2);
				DB->setAuprec(3);
				DB->setLuprec(3);
				DB->setLunits(2);
				DB->setAunits(0);
				DB->setPdmode(35);
				DB->setPdsize(3);
			}
			auto varCD = acDocManager->curDocument();
			if (varCD) {
				acDocManager->sendStringToExecute(
					varCD,
					LR"(vvvvvvattsyngriducsicon
)"
				);
			}
			UpdateLayer::main_do_not_change_text_height();
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


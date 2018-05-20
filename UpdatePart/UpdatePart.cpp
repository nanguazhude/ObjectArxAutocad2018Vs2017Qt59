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
				DB->setDimAssoc(2)/*标注关联*/;
				DB->setAuprec(3)/*角度精度*/;
				DB->setLuprec(3)/*线性精度*/;
				DB->setLunits(2)/*线性小数*/;
				DB->setAunits(0)/*十进制角度*/;
				DB->setPdmode(35)/*设置点样式*/;
				DB->setPdsize(3)/*设置点样式*/;
				DB->setTStackAlign(0);
				DB->setTStackSize(70);
			}
			auto varCD = acDocManager->curDocument();
			if (varCD) {
				acDocManager->sendStringToExecute(
					varCD,
					LR"(vvvvvvattsyngriducsicon
(setvar "PARAMETERCOPYMODE" 3)
(setvar "VTENABLE" 0)
(setvar "FILETABPREVIEW" 0)
(setvar "FILETABTHUMBHOVER" 0)
(setvar "STARTMODE" 0)
(setvar "WIPEOUTFRAME" 2)
(setvar "MSLTSCALE" 1)
(setvar "HPANNOTATIVE" 1)
(setvar "HPLAYER" "细实线")
(setvar "HPNAME" "ANSI31")
(setvar "HPASSOC" 1)
(setvar "CONSTRAINTNAMEFORMAT" 1)
(setvar "MTEXTAUTOSTACK" 1)
(command "FILETAB")
(command "PROPERTIES")
(command "TOOLPALETTES")
(command "RIBBON")
(command "NAVVCUBE" "off" )
(command "NAVBAR" "off" )
(command "layon")
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


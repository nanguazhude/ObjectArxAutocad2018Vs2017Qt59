#include "FEraseAll.hpp"

namespace sstd {
	extern void loadFEraseAll() {
		FEraseAll::load();
	}
}/*namespace sstd*/

void sstd::FEraseAll::load() {
	arx_add_main_command<FEraseAll>();
}

void sstd::FEraseAll::main() {
	//(command "erase" (ssget "x" (list (cons 0 "xline,ray" ) ) ) "" )
	auto varCD = acDocManager->curDocument();
	acDocManager->sendStringToExecute(
		varCD, LR"......((command "erase" (ssget "x" (list (cons 0 "xline,ray" ) ) ) "" )
)......");
}




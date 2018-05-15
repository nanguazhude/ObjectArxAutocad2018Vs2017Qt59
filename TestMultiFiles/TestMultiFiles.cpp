#include "TestMultiFiles.hpp"

namespace sstd {
	template<typename T>
	inline void _arx_add_main_command() {
		acedRegCmds->addCommand(arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_MODAL,
			&T::main
		);
	}
}/*sstd*/

void sstd::TestMultiFiles::load(){
	_arx_add_main_command<TestMultiFiles>();
}

static inline void _sstd_TestMultiFiles_main() {

}

void sstd::TestMultiFiles::main() try {
	_sstd_TestMultiFiles_main();
}
catch (...) {}


/******************************/
namespace sstd{
	extern void loadTestMultiFiles(){
		TestMultiFiles::load();
	}
}/*namespace sstd*/

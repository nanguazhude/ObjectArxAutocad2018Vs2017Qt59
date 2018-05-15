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

void sstd::TestMultiFiles::main(){

}


/******************************/
namespace sstd{
	extern void loadTestMultiFiles(){
		TestMultiFiles::load();
	}
}/*namespace sstd*/

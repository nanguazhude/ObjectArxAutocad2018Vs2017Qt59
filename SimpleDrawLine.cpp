#include "SimpleDrawLine.hpp"

namespace sstd {

	SimpleDrawLine::SimpleDrawLine() {

	}

	void SimpleDrawLine::load() {
		acedRegCmds->addCommand(arx_group_name(),
			globalName(),
			localName(),
			ACRX_CMD_MODAL,
			&SimpleDrawLine::main
		);
	}

	void SimpleDrawLine::main() {

	}

}/*namespace sstd*/

/********************************/


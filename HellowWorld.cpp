#include "HellowWorld.hpp"

namespace sstd {

	HellowWorld::HellowWorld() {
	}

	void HellowWorld::load() {
		acedRegCmds->addCommand(arx_group_name(),
			globalName(),
			localName(),
			ACRX_CMD_MODAL,
			&HellowWorld::main
		);
	}

	void HellowWorld::main() {
		auto varDataTime = QDateTime::currentDateTime().toString().toStdWString();
		varDataTime = LR"(Hellow World!
)" + varDataTime;
		acutPrintf(varDataTime.data());

	}

}/*namespace sstd*/

/********************************/


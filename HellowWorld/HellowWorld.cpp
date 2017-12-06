#include "HellowWorld.hpp"

namespace sstd {

	extern void loadHellowWord() { HellowWorld::load(); }

	HellowWorld::HellowWorld() {
	}

	void HellowWorld::load() {
        arx_add_main_command<HellowWorld>();
	}

	void HellowWorld::main() {

        {
            QtApplication varQApplication ;
            QMessageBox varBox ;
            varBox.setText(VQLS("Hellow World!"));
            varBox.exec() ;
        }

		auto varDataTime = QDateTime::currentDateTime().toString().toStdWString();
		varDataTime = LR"(Hellow World!
)" + varDataTime ;
		acutPrintf(varDataTime.c_str());
		
	}

}/*namespace sstd*/

/********************************/


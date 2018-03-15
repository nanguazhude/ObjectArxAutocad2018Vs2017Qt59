#include "SimpleSelect.hpp"

namespace sstd {
	extern void loadSimpleSelect() {
		SimpleSelect::load();
	}
}

namespace sstd {

	SimpleSelect::SimpleSelect() {
	}

	void SimpleSelect::load() {
		arx_add_main_command_usepickset<SimpleSelect>();
	}

	namespace {
		void _p_select_a_line() {
			class Lock {
			public:
			};
		}
	}

	void SimpleSelect::main() {
		int varFlag = -1;
		/*  */
		acutPrintf(LR"(
1 : 选择一条直线测试
)");

		if (RTNORM == acedGetInt(LR"()", &varFlag)) {
			switch (varFlag)
			{
			case 1:_p_select_a_line(); return; 
			default:break;
			}
		}

		acutPrintf(LR"(输入错误
)");
	}

}/*sstd*/






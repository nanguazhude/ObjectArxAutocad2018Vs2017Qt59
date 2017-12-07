#include "UpdateTitleBlockTime.hpp"

namespace sstd {

	UpdateTitleBlockTime::UpdateTitleBlockTime() {
	}

	extern void loadUpdateTitleBlockTime() { UpdateTitleBlockTime::load(); }
	void UpdateTitleBlockTime::load() {
		arx_add_main_command<UpdateTitleBlockTime>();
	}

	class ThisFunction {
	public:
	};

	void UpdateTitleBlockTime::main() {
		ThisFunction varFunction;
	}
	
}/*namespace sstd*/


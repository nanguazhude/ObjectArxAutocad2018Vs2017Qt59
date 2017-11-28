#include <utility>
#include "ARXApplication.hpp"

/***************************************************/
#include "HellowWorld.hpp"
#include "SimpleDrawLine.hpp"
#include "SimpleDrawLineChangeColor.hpp"
/***************************************************/

namespace sstd {

	namespace {
		using FunctionType = void(*)(void);
		static constexpr FunctionType _v_functions[] = {
			{&HellowWorld::load},
			{&SimpleDrawLine::load},
			{&SimpleDrawLineChangeColor::load},
		};
	}

	void ARXApplication::load() {
		for (const auto & varI : _v_functions) {
			(varI)();
		}
	}

	void ARXApplication::unload() {
		acedRegCmds->removeGroup(arx_group_name());
	}

	ARXApplication::ARXApplication() {

	}

}/*namespace sstd*/

/********************************/


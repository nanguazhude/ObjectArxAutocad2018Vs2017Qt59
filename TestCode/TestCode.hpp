#ifndef TESTCODE_HPP
#define TESTCODE_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class TestCode {
	public:
		TestCode();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_testcode)")
	};

}/*namespace sstd*/

#endif // TESTCODE_HPP

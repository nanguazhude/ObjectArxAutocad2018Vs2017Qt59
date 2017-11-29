#include <string>
#include <string_view>
#include "object_arx_global.hpp"

namespace sstd {

	namespace {
		namespace _cpp_private {
			const constexpr std::string_view qtApplicationPath =
				u8R"(C:\Program Files\Autodesk\AutoCAD 2018\acad.exe)"sv;
			inline int & getArgc() {
				static int ans;
				ans = 1;
				return ans;
			}
			inline char** getArgv() {
				static char acadpath[qtApplicationPath.size() + 4] = {};
				static char *argv[] = { nullptr };
				std::copy(qtApplicationPath.begin(), qtApplicationPath.end(),
					static_cast<char*>(acadpath));
				argv[0] = static_cast<char *>(acadpath);
				return argv;
			}
		}
	}/*namespace*/

	QtApplication::QtApplication() :
		QApplication(_cpp_private::getArgc(), _cpp_private::getArgv()) {
	}

}/*namespace sstd*/




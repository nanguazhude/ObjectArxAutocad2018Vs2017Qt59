#include <string>
#include <string_view>
#include "object_arx_global.hpp"

namespace sstd {

	namespace {
		namespace _cpp_private {
			inline constexpr std::string_view operator""_p_sv(const char * a, std::size_t b) {
				return std::string_view{ a,b };
			}
			const constexpr std::string_view qtApplicationPath =
				u8R"(C:\Program Files\Autodesk\AutoCAD 2018\acad.exe)"_p_sv;
			inline int & getArgc() {
				static int ans;
				ans = 1;
				return ans;
			}
			inline char** getArgv() {
				static std::string acadpath;
				static char *argv[] = { nullptr };
				acadpath = qtApplicationPath;
				argv[0] = const_cast<char *>(acadpath.c_str());
				return argv;
			}
		}
	}/*namespace*/

	QtApplication::QtApplication() :
		QApplication(_cpp_private::getArgc(), _cpp_private::getArgv()) {

	}

}/*namespace sstd*/




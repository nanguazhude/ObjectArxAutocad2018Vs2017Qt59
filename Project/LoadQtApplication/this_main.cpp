#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop)

#include <string_view>
using namespace std::string_view_literals;

#include <QtWidgets>

inline void just_print_hellow_world() {
	acutPrintf(LR"(Hellow Word!
)");
}

namespace {
	namespace _cpp_private {
		const constexpr std::string_view qtApplicationPath =/*!!!*/
#if _OBJECT_ARX_VERSION_X64_ == 2019
			u8R"(C:\Program Files\Autodesk\AutoCAD 2019\acad.exe)"sv;
#else
			u8R"(C:\Program Files\Autodesk\AutoCAD 2018\acad.exe)"sv;
#endif
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

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
	switch (msg) {
	case AcRx::kInitAppMsg: {
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		/*****************************************/
		{
			if (qApp == nullptr) {
				/*create the qt applicaton and never destory it*/
				auto varQtApplication =
					new QApplication(_cpp_private::getArgc(), _cpp_private::getArgv());
				(void)varQtApplication;
			}
			/*force to load images plugins*/
			QImage varImage{ QString(":/png/this.png") };
			varImage.width();
			varImage.height();
		}
		/*****************************************/
		acedRegCmds->addCommand(
			L"SSTD_GLOBAL_CMD_GROUP",
			L"_just_print_hellow_world",
			L"just_print_hellow_world",
			ACRX_CMD_MODAL,
			&just_print_hellow_world);
	}break;
	case AcRx::kUnloadAppMsg: {}break;
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


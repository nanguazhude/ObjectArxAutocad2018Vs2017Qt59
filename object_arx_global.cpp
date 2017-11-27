#include "object_arx_global.hpp"
#include "ARXApplication.hpp"

extern "C" LIBRARY_DEFINE AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
	switch (msg) {
	case AcRx::kInitAppMsg: {
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		sstd::ARXApplication::load();
		break;
	}
	case AcRx::kUnloadAppMsg: {
		sstd::ARXApplication::unload();
		break;
	}
	default:break;
	}

	return AcRx::kRetOK;
}

/********************************/



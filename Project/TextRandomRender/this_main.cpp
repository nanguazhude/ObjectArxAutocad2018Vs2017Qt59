#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop )

#include "text_render.hpp"
extern void text_render(sstd::RenderState *);

static inline void main_text_render() {
	sstd::RenderState varRenderState;
	text_render(&varRenderState);
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
	switch (msg) {
	case AcRx::kInitAppMsg: {
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		acedRegCmds->addCommand(
			L"SSTD_GLOBAL_CMD_GROUP",
			L"_shxtextview",
			L"shxtextview",
			ACRX_CMD_MODAL,
			&main_text_render);
	}break;
	case AcRx::kUnloadAppMsg: {}break;
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


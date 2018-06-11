#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop )

#include <memory>
#include "text_render.hpp"
extern void text_render(sstd::RenderState *);

namespace {
	template<typename T>
	class UniquePtr : public std::unique_ptr<T> {
		using Parent = std::unique_ptr<T>;
	public:
		UniquePtr(T * arg) :Parent(arg) {}
	};
}/*namespace*/

static inline void main_text_render() try{
	UniquePtr varRenderState{ new sstd::RenderState };
	while (varRenderState->$IsEndl == false) {
		text_render(varRenderState.get());
	}
}
catch (...) {
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


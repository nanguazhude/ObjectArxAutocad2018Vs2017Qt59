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

	class DocumentLock {
	public:
		DocumentLock() {
			acDocManager->lockDocument(acDocManager->curDocument());
		}
		~DocumentLock() {
			acDocManager->unlockDocument(acDocManager->curDocument());
		}
	};

}/*namespace*/

static inline void init_state(sstd::RenderState *arg) {
	DocumentLock varLock;

}

static inline void next_state(sstd::RenderState *arg) {
	acDocManager->appContextNewDocument(LR"(@acadISO -Named Plot Styles.dwt)");
	acDocManager->lockDocument(acDocManager->curDocument());
}

static inline void main_text_render() try {
	UniquePtr varRenderState{ new sstd::RenderState };
	init_state(varRenderState.get());
	while (varRenderState->$IsEndl == false) {
		next_state(varRenderState.get());
		text_render(varRenderState.get());
		/*save the stete */
		{
			auto varSavePath = std::filesystem::u8path(varRenderState->$CurrentPageNumber.$String.data());
			varSavePath += ".dwg"sv;
			varSavePath = varRenderState->$OutPutDir / varSavePath;
			acdbHostApplicationServices()
				->workingDatabase()
				->saveAs(varSavePath.wstring().c_str())/*保存文档*/;
		}
		acDocManager->unlockDocument(acDocManager->curDocument());
		acDocManager->closeDocument(acDocManager->curDocument())/*关闭文档*/;
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
			ACRX_CMD_SESSION,
			&main_text_render);
	}break;
	case AcRx::kUnloadAppMsg: {}break;
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


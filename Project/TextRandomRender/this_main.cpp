#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop )

#include <memory>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qfiledialog.h>
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

	{/*get input file name and output dir name*/
		if (qApp == nullptr) {/*this block should just run once or never run*/
			static char args_0[] = u8R"(C:\Program Files\Autodesk\AutoCAD 2019\acad.exe)";
			static char * args[] = { args_0,nullptr };
			static int argc = 1;
			static UniquePtr app = new QApplication{ argc, static_cast<char **>(args) };
		}
		const QString varFileName = QFileDialog::getOpenFileName();
		if (varFileName.isEmpty()) { throw std::runtime_error("can not find input file"); }
		const QString varDirName = QFileDialog::getExistingDirectory();
		if (varDirName.isEmpty()) { throw std::runtime_error("can not find output dir"); }
		arg->$OutPutDir = std::filesystem::u8path(varDirName.toUtf8().toStdString());
		arg->$TextFileName = varFileName;
	}

	{/*set font names*/
		if constexpr(false) {
			arg->$Fonts.emplace_back(QStringLiteral("陈静的字完整版"), 1.2);
			arg->$Fonts.emplace_back(QStringLiteral("萌妹子体"), 1.0);
			arg->$Fonts.emplace_back(QStringLiteral("李国夫手写体"), 1.0);
		}
		else {
			arg->$Fonts.emplace_back(QStringLiteral("宋体"), 1.0);
			arg->$Fonts.emplace_back(QStringLiteral("楷体"), 1.1);
			arg->$Fonts.emplace_back(QStringLiteral("仿宋"), 1.0);
		}
	}

}

static inline AcApDocument*  next_state(sstd::RenderState *arg) {
	acDocManager->appContextNewDocument(LR"(@acadISO -Named Plot Styles.dwt)");
	auto varAns = acDocManager->curDocument();
	acDocManager->lockDocument(varAns);
	return varAns;
	(void)arg;
}

static inline void main_text_render() try {
	UniquePtr varRenderState{ new sstd::RenderState };
	init_state(varRenderState.get());
	while (varRenderState->$IsEndl == false) {
		auto varCD = next_state(varRenderState.get());
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
		acDocManager->unlockDocument(varCD);
		acDocManager->appContextCloseDocument(varCD)/*关闭文档*/;
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
			L"_textrandomrender",
			L"textrandomrender",
			ACRX_CMD_SESSION,
			&main_text_render);
	}break;
	case AcRx::kUnloadAppMsg: {}break;
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


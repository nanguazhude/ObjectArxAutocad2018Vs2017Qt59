#include "TestCode.hpp"
#include <acedCmdNF.h>

namespace sstd {

	extern void loadTestCode() { TestCode::load(); }

	TestCode::TestCode() {

	}

	void TestCode::load() {
		arx_add_main_command<TestCode>();
	}

	namespace {
		inline Acad::ErrorStatus openFile(const std::wstring_view & argFileName) {
			Acad::ErrorStatus varE =
				acDocManager->appContextOpenDocument(argFileName.data());
			if (varE != Acad::eOk) {
				acutPrintf(LR"(打开文件失败:)");
				acutPrintf(acadErrorStatusText(varE));
				acutPrintf(LR"(:)");
				acutPrintf(argFileName.data());
				acutPrintf(LR"(
)");
			}
			return varE;
		}

		class ThisReactor :public AcApDocManagerReactor {
		public:

			void documentBecameCurrent(AcApDocument* arg) override{
				const wchar_t * varFileName;
				arg->database()->getFilename(varFileName);
				acutPrintf(varFileName);
				acutPrintf(LR"(X*****************************************
)");
				
				/*acedCommandS(RTSTR, LR"(CIRCLE)",
					RTSTR, LR"(0,0,0)",
					RTSTR, LR"(100)",
					RTNONE);*/

			}

			void documentActivated(AcApDocument* arg) override {
				
				const wchar_t * varFileName;
				arg->database()->getFilename(varFileName);
				
				QFileInfo varFile{ QString::fromWCharArray(varFileName) };
				const auto varFileNameQ = varFile.fileName();
				const auto varFileNameW = varFileNameQ.toStdWString();
				acutPrintf(varFileNameW.data());
				if ( varFileNameQ == VQLS("template.all.1.dwg")) {
					 
					acutPrintf(LR"(----------)");
								

				}

			}
		};

	}/*namespace*/

	void TestCode::main() {

		auto r = new ThisReactor;
		acDocManager->addReactor(r);
		acDocManager->executeInApplicationContext([](void*) {

			const auto varFileName = LR"(E:\Duty\Duty\template\template.all.1.dwg)"sv;
			if (Acad::eOk != openFile(varFileName)) {
				return;
			}		
			
		//acDocManager->closeDocument( acDocManager->curDocument() );
		}, nullptr);

		//const auto varStartTime = std::chrono::high_resolution_clock::now();
		//for (;;) {
		//	const wchar_t * filename;
		//	acDocManager->curDocument()->database()->getFilename(filename);
		//	QString varFilrName = QString::fromWCharArray(filename);
		//	if (varFilrName.endsWith("dwg")) { break; }
		//	if ((std::chrono::high_resolution_clock::now() - varStartTime)>10s) {
		//		break;
		//	}
		//}

		//// RTNORM
		//acedCommandS(RTSTR, LR"(CIRCLE)",
		//	RTSTR, LR"(0,0,0)"          ,
		//	RTSTR, LR"(100)"            ,
		//	RTNONE);

//
//		{
//			auto varDB = acDocManager->curDocument()->database();
//			if (Acad::eOk != varDB->saveAs(varFileName.data())) {
//				acutPrintf(LR"(保存文件失败
//)");
//			}
//		}

	}

}/*namespace sstd*/




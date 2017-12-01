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
			//bool varIsRun = false;
			int $count = 0;
		public:

			void documentBecameCurrent(AcApDocument* arg) override {
				//if (varIsRun) {
				//	acDocManager->removeReactor(this);
				//	delete this;
				//}
				const wchar_t * varFileName;
				arg->database()->getFilename(varFileName);
				acutPrintf(varFileName);
				acutPrintf(LR"(X*****************************************
)");
				
				acDocManager->curDocument()->database()->getFilename(varFileName);
				acutPrintf(varFileName);
				acutPrintf(LR"(X??????????????????????????????????????????
)");

				QFileInfo varFile{ QString::fromWCharArray(varFileName) };
				const auto varFileNameQ = varFile.fileName();
				if ( varFileNameQ.endsWith("dwg")  ) {
					auto var = QString::number($count++).repeated(10).toStdWString();
					acutPrintf(acDocManager->isApplicationContext() ?
						LR"(true)":LR"(false)"
					);
					acutPrintf(var.data());
				}
				//varIsRun = true;

				//acDocManager->lockDocument(arg);
				acutPrintf(arg->isCommandInputInactive() ? LR"(YYY)" : LR"(NNN)");
				//acedCommandS(RTSTR, LR"(CIRCLE)",
				//RTSTR, LR"(0,0,0)",
				//RTSTR, LR"(100)",
				//RTNONE);
				//acDocManager->unlockDocument(arg);
				//acDocManager->sendStringToExecute(arg, LR"(circle 0,0,0 10 )");
				//acDocManager->sendStringToExecute(arg,
				//	LR"(circle 0,0,0 10 )"
                //    LR"(QSAVE )"
				//	LR"(CLOSE )"
				//	);
				//AcEdInputPointManager
				/*acDocManager-> executeInApplicationContext(
					[](void *) {
					acedCommandS(RTSTR, LR"(CIRCLE)",
						RTSTR, LR"(0,0,0)",
						RTSTR, LR"(100)",
						RTNONE);
				},nullptr
				);*/

			}

			void documentActivated(AcApDocument* arg) override {

				const wchar_t * varFileName;
				arg->database()->getFilename(varFileName);

				QFileInfo varFile{ QString::fromWCharArray(varFileName) };
				const auto varFileNameQ = varFile.fileName();
				const auto varFileNameW = varFileNameQ.toStdWString();
				acutPrintf(varFileNameW.data());
				if (varFileNameQ == VQLS("template.all.1.dwg")) {

					acutPrintf(LR"(----------)");


				}

			}
		};

		class Y :public AcEdInputContextReactor {};

	}/*namespace*/

	void TestCode::main() {
		//acedEditor->addReactor(new Y);

		//auto r = new ThisReactor;
		//acDocManager->addReactor(r);
		acDocManager->executeInApplicationContext([](void*) {

			const auto varFileName = LR"(E:\Duty\Duty\template\template.all.1.dwg)"sv;
			if (Acad::eOk != openFile(varFileName)) {
				return;
			}

			//acDocManager->closeDocument( acDocManager->curDocument() );
		},nullptr );

		acutPrintf( LR"(
D
)" );
		acutPrintf( acDocManager->curDocument()->fileName() );
		AcApDocumentIterator* varDI = acDocManager->newAcApDocumentIterator();
		if (varDI) {
			for (;!varDI->done();varDI->step()) {
				acutPrintf(LR"(
V
)");
				acutPrintf( varDI->document()->fileName() );
				if (QString::fromWCharArray(
					varDI->document()->fileName()).endsWith("dwg")) {

					auto varD = varDI->document();
					if (acDocManager->lockDocument(varD)==Acad::eOk) {
						if (acDocManager->activateDocument(varD)==Acad::eOk) {
							if (acDocManager->setCurDocument(varD) == Acad::eOk) {
								acedCommandS(RTSTR, LR"(CIRCLE)",
									RTSTR, LR"(0,0,0)",
									RTSTR, LR"(100)",
									RTNONE);
							}
							else {
								acutPrintf(LR"(
SETCURRENT FAILED
)");
							}
						}
						else {
							acutPrintf(LR"(
DWGACT FAILED
)");
						}
					}
					else {
						acutPrintf(LR"(
DWGLOCKED FAILED
)");
					}


				}

			}
		}
		


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




#include "TestCode.hpp"
#include <acedCmdNF.h>
//AcApCommandLineEditor

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

			
				 

	}/*namespace*/

	void TestCode::main() {
		
		auto varCurrentDocument = acDocManager->curDocument();
		const static constexpr auto varFileName = LR"(D:\test1\drawing2.dwg)"sv;
		
		bool varIsOk = false;
		acDocManager->executeInApplicationContext([](void * argIsOk) {
			*reinterpret_cast<bool*>(argIsOk) = (Acad::eOk == openFile(varFileName.data()));
		},&varIsOk);

		if (varIsOk) {

			std::unique_ptr<AcApDocumentIterator> varIt{ acDocManager->newAcApDocumentIterator() };

			for (;!varIt->done();varIt->step() ) {
				auto varDoc = varIt->document();
				if ((varDoc==nullptr)||(varDoc == varCurrentDocument)) { continue; }
				acDocManager->sendStringToExecute(varDoc,LR"(circle
0,0,0
100.58
)"
					LR"(qsave
)"
					LR"(close
)"
				);
				//acDocManager->sendStringToExecute(varDoc,LR"( qsave)");
				//acDocManager->sendStringToExecute(varDoc,LR"( close)");
			}
						
		}
				
	}

}/*namespace sstd*/




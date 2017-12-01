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

			
				 

	}/*namespace*/

	void TestCode::main() {
		
		/*acDocManager->closeDocument( acDocManager->curDocument() );*/

		const auto varFileName = LR"(E:\Duty\Duty\template\template.all.1.dwg)"sv;
		/*if (Acad::eOk != openFile(varFileName)) {
			return;
		}*/

		auto db = new AcDbDatabase{ false };
		db->readDwgFile(varFileName.data());
		auto varDB = acDocManager->document(db);

		acDocManager->setCurDocument(varDB);
		acDocManager->activateDocument(varDB);

	}

}/*namespace sstd*/




#include "TestMultiFiles.hpp"

namespace sstd {
	template<typename T>
	inline void _arx_add_main_command() {
		acedRegCmds->addCommand(arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_SESSION,
			&T::main
		);
	}
}/*sstd*/

void sstd::TestMultiFiles::load(){
	_arx_add_main_command<TestMultiFiles>();
}

static inline void _sstd_TestMultiFiles_main() {

	class DocumentLock {
	public:
		DocumentLock() {
			acDocManager->lockDocument(acDocManager->curDocument());
		}
		~DocumentLock() {
			acDocManager->unlockDocument(acDocManager->curDocument());
		}
	};

	//application  
	
	{//do somenthing int the Autocad opened documented
		DocumentLock varDocumentLock;
		auto varDataBase = acdbHostApplicationServices()->workingDatabase();
		const wchar_t * varFileName;
		varDataBase->getFilename(varFileName);
		acutPrintf(LR"(CurrentDocument: %s
)", varFileName);
		acutPrintf(LR"(CurrentDocumentFileName: %s
)", acDocManager->curDocument()->fileName());
	}

	//application
	//new a document ... 
	acDocManager->appContextNewDocument(LR"(@acadISO -Named Plot Styles.dwt)");
	//appContextOpenDocument
	//appContextRecoverDocument

	{//do something on it
		DocumentLock varDocumentLock;
		auto varDataBase = acdbHostApplicationServices()->workingDatabase();
		const wchar_t * varFileName;
		varDataBase->getFilename(varFileName);
		acutPrintf(LR"(CurrentDocument: %s
)", varFileName);
		acutPrintf(LR"(CurrentDocumentFileName: %s
)", acDocManager->curDocument()->fileName());
		//save the document
		varDataBase->saveAs(LR"(D:/Project1/project.dwg)");
	}

	//application
	acDocManager->closeDocument( acDocManager->curDocument() );

	/*********************/
	//AcApDocument *doc = acDocManager->document(dwg);
	//https://forums.autodesk.com/t5/objectarx/getfilename/m-p/313975

}

void sstd::TestMultiFiles::main() try {
	_sstd_TestMultiFiles_main();
}
catch (...) {}


/******************************/
namespace sstd{
	extern void loadTestMultiFiles(){
		TestMultiFiles::load();
	}
}/*namespace sstd*/

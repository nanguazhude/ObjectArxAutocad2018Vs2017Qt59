#include "TestMultiFiles.hpp"
#include <filesystem>

static inline bool save( AcDbDatabase * doc , const wchar_t * full_name)try {
	{
		const std::filesystem::path varFilePath(full_name);
		if (false == std::filesystem::remove(varFilePath)) {
			svthrow(LR"(can not remove file!)");
		}
	}
	return eOk==doc->saveAs(full_name);
}
catch (...) {
	return false;
}

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
		acutPrintf(LR"(%s
)" , acDocManager->curDocument()->isNamedDrawing()?LR"(true)": LR"(false)");
		//save the document
		save(varDataBase,LR"(D:/Project1/project.dwg)");
		acutPrintf(LR"(%s
)", acDocManager->curDocument()->isNamedDrawing() ? LR"(true)" : LR"(false)");
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

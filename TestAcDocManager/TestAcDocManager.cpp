#include "TestAcDocManager.hpp"

namespace sstd {

	extern void loadTestAcDocManager() { TestAcDocManager::load(); }

	TestAcDocManager::TestAcDocManager() {

	}

	void TestAcDocManager::load() {
		arx_add_main_command<TestAcDocManager>();
	}

	class DocumentLock {
		AcApDocument * $old_document = nullptr;
		AcApDocument * $cur_document = nullptr;
		bool $isDocumentLocked = false;
	public:
		DocumentLock() = delete;
		DocumentLock(const DocumentLock &&) = delete;
		DocumentLock(DocumentLock &&) = delete;
		DocumentLock&operator=(const DocumentLock &&) = delete;
		DocumentLock&operator=(DocumentLock &&) = delete;

		inline DocumentLock(AcApDocument *arg) {
			auto varCD = acDocManager->curDocument();
			if ((arg == varCD) || (arg == nullptr)) { return; }
			$old_document = varCD;
			$cur_document = arg;
		}

		inline DocumentLock(AcDbDatabase *arg) :
			DocumentLock(arg ? acDocManager->document(arg) : nullptr) {
		}

		inline Acad::ErrorStatus lockAndSetCurrent() {
			Acad::ErrorStatus varE =
				acDocManager->lockDocument($cur_document);
			if (varE != Acad::eOk) { return varE; }
			$isDocumentLocked = true;
			varE = acDocManager->setCurDocument($cur_document);
			if (varE == Acad::eOk) {
				return acDocManager->activateDocument($cur_document);
			}
			return varE;
		}

		inline ~DocumentLock() {
			if ($cur_document&&$isDocumentLocked) {
				acDocManager->unlockDocument($cur_document);
			}
			if ($old_document && ($old_document != acDocManager->curDocument())) {
				acDocManager->setCurDocument($old_document);
				acDocManager->activateDocument($old_document);
			}
		}

	};

	void TestAcDocManager::main() {

		/*open file*/
		constexpr const auto varTargetFileName =
			LR"(E:\Duty\Duty\template\template.all.1.dwg)"sv;

		/*set file to current document*/

		/*run some command*/

		/*save the document*/

	}

}/*namespace sstd*/






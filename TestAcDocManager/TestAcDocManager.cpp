#include "TestAcDocManager.hpp"

namespace sstd {

	extern void loadTestAcDocManager() { TestAcDocManager::load(); }

	TestAcDocManager::TestAcDocManager() {

	}

	void TestAcDocManager::load() {
		arx_add_main_command<TestAcDocManager>();
	}

	class DocumentLock {
		AcApDocument * $olde_document=nullptr;
	public:
		DocumentLock() = delete;
		
	};

	void TestAcDocManager::main() {

		acDocManager->curDocument();


	}

}/*namespace sstd*/






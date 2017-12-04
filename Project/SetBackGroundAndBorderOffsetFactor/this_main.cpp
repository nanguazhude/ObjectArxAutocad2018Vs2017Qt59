#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop)

class EditorReactor :
	public AcEditorReactor {
public:
	inline void databaseConstructed(AcDbDatabase*);
};

class DatabaseReacotr :
	public AcDbDatabaseReactor {
public:
	inline void objectAppended(
		const AcDbDatabase*,
		const AcDbObject* dbObj) override {
		auto varMtext = AcDbMText::cast(dbObj);
		if (varMtext) {
			double varBF = 0;
			varMtext->getBackgroundScaleFactor(varBF);
			if (std::abs(varBF - 1.005) < 0.0001) {
				return;
			}

			auto & var = varMtext;
			if (Acad::eOk == var->upgradeOpen()) {
				var->setBackgroundFill(true);
				var->setUseBackgroundColor(true);
				var->setBackgroundScaleFactor(1.005);
			}
		}
	}

};

DatabaseReacotr * globalDatabaseReacotr = 0;
EditorReactor *   globalEditorReactor   = 0;

void EditorReactor::databaseConstructed(AcDbDatabase* var) {
	var->addReactor(globalDatabaseReacotr);
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
	switch (msg) {
	case AcRx::kInitAppMsg: {
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		/*****************************************/
		globalDatabaseReacotr = new DatabaseReacotr;
		globalEditorReactor = new EditorReactor;
		acedEditor->addReactor(globalEditorReactor);
		/*****************************************/
		break;
	}
	case AcRx::kUnloadAppMsg: {
		break;
	}
	default:break;
	}

	return AcRx::kRetOK;
}

/********************************/


#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop)

static bool globalAddFlage = false;
inline void setAutoAddMaskToMText() { globalAddFlage = true; }
inline void clearAutoAddMaskToMText() { globalAddFlage = false; }

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
		const AcDbObject* dbObj) /*override*/ {
		if (false == globalAddFlage) { 
			return; 
		}
		AcDbMText * varMtext = AcDbMText::cast(dbObj);
		if (varMtext) {
			double varBF = 0;
			varMtext->getBackgroundScaleFactor(varBF);
			if (std::abs(varBF - 1.075) < 0.0001) {
				return;
			}

			if (Acad::eOk == varMtext->upgradeOpen()) {
				varMtext->setBackgroundFill(true);
				varMtext->setUseBackgroundColor(true);
				varMtext->setBackgroundScaleFactor(1.075);
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
		acedRegCmds->addCommand(
			L"SSTD_GLOBAL_CMD_GROUP", 
			L"_setAutoAddMaskToMText", 
			L"setAutoAddMaskToMText", 
			ACRX_CMD_MODAL , 
			&setAutoAddMaskToMText);
		acedRegCmds->addCommand(
			L"SSTD_GLOBAL_CMD_GROUP",
			L"_clearAutoAddMaskToMText",
			L"clearAutoAddMaskToMText",
			ACRX_CMD_MODAL,
			&clearAutoAddMaskToMText);
		break;
	}
	case AcRx::kUnloadAppMsg: {break;}
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop)

#include <string_view>
using namespace std::string_view_literals;

#include <QtWidgets/QtWidgets>
#include <QtQml>
#include <vector>
#include <forward_list>
#include <string_view>

using namespace std::string_view_literals;

class Item {
public:
	QFileInfo file_info;
};

/****************************************/
AcDbMText * drawitem ( Item * i , int n  )   {
	AcDbMText * varItem = new AcDbMText;
	std::wstring varData;
	{
		varData = i->file_info.fileName().toStdWString();
		varData += LR"b...e( 1沉孔2锪34%%p56%%d78×9今天0)b...e"sv;
	}
	varItem->setLocation({ 0, n * (-18.0) ,0 });
	 
	varItem->setContents(varData.c_str());
	
	return varItem;
}

template<typename T,typename T1,typename B>
void drawitems(T b,const T1 e,B d) {
	int n = -1;
	for (;b!=e;++b) {
		auto v = drawitem(*b,++n);
		d->appendAcDbEntity(v);
		v->close();
	}
}

inline static void show_shx_text() {
	std::forward_list< Item > var_memory_list;
	std::vector< Item * > varItems;
	const QString & varDirName = QFileDialog::getExistingDirectory();
	if (varDirName.isEmpty()) {
		return;
	}
	/**/
	QDirIterator varIt{ varDirName,
		QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::NoSymLinks,
		QDirIterator::NoIteratorFlags };

	while (varIt.hasNext()) {
		varIt.next();
		const auto varFileInfo = varIt.fileInfo();
		if (varFileInfo.suffix().toLower() == QStringLiteral("shx")) {
			Item * i = &var_memory_list.emplace_front();
			i->file_info = varFileInfo;
			varItems.push_back(i);
		}
	}

	/*draw items*/
	AcDbBlockTableRecord * pRec ;
	{
		const auto varDB = acdbHostApplicationServices()->workingDatabase();

		AcDbBlockTable * pBlkTable;
		if (varDB->getBlockTable(pBlkTable, AcDb::kForRead) != Acad::eOk)
			return;

		if (pBlkTable->getAt(ACDB_MODEL_SPACE, pRec, AcDb::kForWrite) != Acad::eOk)
			return;
		pBlkTable->close();
	}

	try {
		drawitems(varItems.cbegin(), varItems.cend(), pRec);
	}
	catch (...) {
	}
	pRec->close();
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
	switch (msg) {
	case AcRx::kInitAppMsg: {
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);

		acedRegCmds->addCommand(
			L"SSTD_GLOBAL_CMD_GROUP",
			L"_shxtextview",
			L"shxtextview",
			ACRX_CMD_MODAL,
			&show_shx_text);

	}break;
	case AcRx::kUnloadAppMsg: {}break;
	default:break;
	}
	return AcRx::kRetOK;
}

/********************************/


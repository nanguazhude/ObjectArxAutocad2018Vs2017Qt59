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
#include <regex>

using namespace std::string_view_literals;

class Item {
public:
	QFileInfo file_info;
	AcDbObjectId style_index;
};

/****************************************/
AcDbMText * drawitem(Item * i, int n) {
	AcDbMText * varItem = new AcDbMText;
	std::wstring varData;
	{
		varData = i->file_info.fileName().toStdWString();
		varData += LR"b...e( 1沉孔2锪34%%p56%%d78×9今天0)b...e"sv;
	}
	varItem->setLocation({ 0, n * (-18.0) ,0 });
	varItem->setTextStyle(i->style_index);
	varItem->setWidth(1000);
	varItem->setColumnWidth(1000);
	varItem->setContents(varData.c_str());

	return varItem;
}

template<typename T, typename T1, typename B>
void drawitems(T b, const T1 e, B d) {
	int n = -1;
	for (; b != e; ++b) {
		auto v = drawitem(*b, ++n);
		d->appendAcDbEntity(v);
		v->close();
	}
}

void create_text_style(Item * i,int n){
	auto varDB = acdbHostApplicationServices()->workingDatabase();
	AcDbTextStyleTable* varTST = nullptr;
	if(eOk!= varDB->getTextStyleTable(varTST,AcDb::kForWrite))
		return;
	std::wstring varStyleName{ LR"(text_style_name_)"sv };
	{
		auto varNumber = std::to_wstring(n);
		switch (varNumber.size())
		{
		case 0:break;
		case 1:varStyleName += LR"(00000)"sv; break;
		case 2:varStyleName += LR"(0000)"sv; break;
		case 3:varStyleName += LR"(000)"sv; break;
		case 4:varStyleName += LR"(00)"sv; break;
		case 5:varStyleName += LR"(0)"sv; break;
		}
		varStyleName += std::move(varNumber);
	}
	
	AcDbTextStyleTableRecord * varR = nullptr ;
	bool toInsert = false;
	if (toInsert=(eOk == varTST->getAt(varStyleName.c_str(), i->style_index))) {
		if (eOk != acdbOpenObject(varR, i->style_index, AcDb::kForWrite)) {
			varTST->close(); 
			return;
		}
	}

	toInsert = !toInsert;

	if (toInsert) { 
		varR = new AcDbTextStyleTableRecord; 
	}

	varR->setName(varStyleName.c_str());
	varR->setFileName(LR"(ztxt.shx)");
	{
		const auto varFN = i->file_info.absoluteFilePath().toStdWString();
		varR->setBigFontFileName( varFN.c_str() );
	}

	if (toInsert) {
		varTST->add(i->style_index, varR);
	}

	varR->close();
	varTST->close();
}

template<typename T, typename T1 >
void create_text_styles(T b, const T1 e) {
	int n = -1;
	for (; b != e; ++b) {
		create_text_style(*b ,++n);
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
		QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files,
		QDirIterator::NoIteratorFlags };

	while (varIt.hasNext()) {
		varIt.next();
		const auto varFileInfo = varIt.fileInfo();
		if (varFileInfo.suffix().toLower() == QStringLiteral("shx")) {

			{/*检查当前文件是否是大字体文件*/
				QFile varFile{ varFileInfo.absoluteFilePath() };
				if (false == varFile.open(QIODevice::ReadOnly))
					continue;
				constexpr const static std::size_t size = 32;
				char varTmp[size];
				auto const varDataSize = varFile.read(varTmp, size);
				if (varDataSize < size) { continue; }
				const std::string_view view{ varTmp ,size };
				const static std::regex varR(u8R"(bigfont)", std::regex::icase);
				if (false == std::regex_search(view.begin(), view.end(), varR))
					continue;
			}

			Item * i = &var_memory_list.emplace_front();
			i->file_info = varFileInfo;
			varItems.push_back(i);
			if (varItems.size() > 1 ) { break; }
		}
	}
	/*cretae text styles*/
	{
		create_text_styles(varItems.cbegin(), varItems.cend());
	}
	/*draw items*/
	AcDbBlockTableRecord * pRec;
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


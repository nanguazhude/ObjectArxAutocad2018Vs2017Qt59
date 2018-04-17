#include "EText.hpp"

void sstd::EText::load(){
    arx_add_main_command<EText>();
}

void sstd::EText::main()try{
	QtApplication varQtApp_;
	QString varFileNameQ;
	{
		const wchar_t * varFileName
			= acDocManager
			->mdiActiveDocument()
			->fileName();
		if (varFileName == nullptr) { throw 6; }
		varFileNameQ = QString::fromWCharArray(varFileName);
	}

	const auto varPath = QFileInfo(varFileNameQ).absolutePath();
	
	QString varFullFileName;
	{
		QDir varDir{varPath};
		varFullFileName = varDir.absoluteFilePath("xy.wh.txt");
	}

	if ( false == QFile::exists(varFullFileName) ) {
		QFile varFile{ varFullFileName };
		if (false == varFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			acutPrintf(LR"(can not creat file
)");
			throw 333;
		}
		QTextStream varStream{ &varFile };
		varStream.setCodec( QTextCodec::codecForName("UTF-8") );
		varStream.setGenerateByteOrderMark(true);

		varStream << endl;

	}

	QDesktopServices::openUrl(QUrl::fromLocalFile(varFullFileName));

}
catch (...) {}

namespace sstd {

extern void loadEText(){
    EText::load();
}

}/*namespace sstd*/


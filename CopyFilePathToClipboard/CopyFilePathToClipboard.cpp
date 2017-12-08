#include <object_arx_global.hpp>
#include "CopyFilePathToClipboard.hpp"

namespace sstd {

	CopyFilePathToClipboard::CopyFilePathToClipboard() {

	}

	extern void loadCopyFilePathToClipboard() { CopyFilePathToClipboard::load(); }

	void CopyFilePathToClipboard::load() {
		arx_add_main_command<CopyFilePathToClipboard>();
	}

	void CopyFilePathToClipboard::main() {
		auto varDB = acdbHostApplicationServices()
			->workingDatabase();
		QString varFileNameQ;
		{
			const wchar_t * varFileName;
			varDB->getFilename(varFileName);
			varFileNameQ.fromWCharArray(varFileName);
		}
		QClipboard *varClipborad = QApplication::clipboard();
		const auto varFilePath = QFileInfo(varFileNameQ).absolutePath();
		varClipborad->setText(varFilePath);
		acutPrintf(LR"(当前文件路径：)");
		const auto varFilePathW = varFilePath.toStdWString();
		acutPrintf(get_cstr(varFilePathW));
		acutPrintf(LR"(
)");
	}

}/*namespace sstd*/


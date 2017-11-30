#include "UpdateBlockFromOtherFile.hpp"

namespace sstd {

	extern void loadUpdateBlockFromOtherFile() { UpdateBlockFromOtherFile::load(); }

	void UpdateBlockFromOtherFile::load() {
		arx_add_main_command<UpdateBlockFromOtherFile>();
	}

	void UpdateBlockFromOtherFile::main() {
		constexpr const auto varOtherFileName = LR"(E:\Duty\Duty\template\template.all.dwg)"sv;
		constexpr const auto varBlockName = LR"(横边框2(G3000))"sv;
		constexpr const auto varTargetFileName = LR"(E:\Duty\Duty\template\template.all.1.dwg)"sv;
		
		std::unique_ptr<AcDbDatabase> varTargetFile{ new AcDbDatabase{ false } };
		if (Acad::eOk != varTargetFile->readDwgFile(varTargetFileName.data(),
			AcDbDatabase::kForReadAndWriteNoShare)) {
			acutPrintf(LR"(打开文件")");
			acutPrintf(varTargetFileName.data());
			acutPrintf(LR"("失败
)");
			return;
		}
		else {
			acutPrintf(LR"(打开文件")");
			acutPrintf(varTargetFileName.data());
			acutPrintf(LR"("成功
)");
		}

		std::unique_ptr<AcDbDatabase> varOtherFile{ new AcDbDatabase{false} };
		if (Acad::eOk != varOtherFile->readDwgFile(varOtherFileName.data())) {
			acutPrintf(LR"(打开文件")");
			acutPrintf(varOtherFileName.data());
			acutPrintf(LR"("失败
)");
			return;
		}			

		/*{
			AcDbObjectId varBlockID;
			if (Acad::eOk != varTargetFile->insert(varBlockID,
				varBlockName.data(),
				varOtherFile.get(),
				true)) {
				acutPrintf(LR"(插入文件失败
)");
				return;
			}
		}*/

		if (Acad::eOk != varTargetFile->save()) {
			acutPrintf(LR"(保存文件失败
)");
			return;
		}
		else {
			acutPrintf(LR"(保存文件成功
)");
		}
		
	}/*UpdateBlockFromOtherFile::main*/

	UpdateBlockFromOtherFile::UpdateBlockFromOtherFile() {

	}

}/*namespace sstd*/



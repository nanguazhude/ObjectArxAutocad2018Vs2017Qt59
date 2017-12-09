#include "UpdateBlockFromOtherFile.hpp"

namespace sstd {

	extern void loadUpdateBlockFromOtherFile() { UpdateBlockFromOtherFile::load(); }

	void UpdateBlockFromOtherFile::load() {
		arx_add_main_command<UpdateBlockFromOtherFile>();
	}

	namespace {
		class ThisFunction final {
		public:
			AcDbDatabase *const $TargetDataBase;
			static std::wstring $BlockSearchDir;
			using BlockFiles = std::pair< const std::pair<
				std::wstring/*filename*/,
				std::wstring_view/*blockname*/ > *,
				int >;
			inline void update();
			ThisFunction(AcDbDatabase * arg) :$TargetDataBase(arg) {}
		private:
			inline void _p_updateBlockSearchDir();
			static inline BlockFiles _p_getBlockFiles();
		};

		template<typename T> class ArraySize;
		template<typename T, int N>
		class ArraySize<T[N]> {
		public:
			constexpr static int value() { return N; }
		};

		std::wstring ThisFunction::$BlockSearchDir;
		inline ThisFunction::BlockFiles ThisFunction::_p_getBlockFiles() {
			static const std::pair<
				std::wstring/*filename*/,
				std::wstring_view/*blockname*/ > varData[] = {
					{ LR"(_MY_ROW.dwg)"s,LR"(_MY_ROW)"sv }/*this must be the first*/,
					{ LR"(横边框2(G3000).dwg)"s,LR"(横边框2(G3000))"sv },
					{ LR"(_毂形位公差.dwg)"s,LR"(@毂形位公差.111133)"sv },
					{ LR"(_刹车面形位公差.dwg)"s,LR"(@刹车面形位公差.111133)"sv },
					{ LR"(@标题栏_粗加工.dwg)"s,LR"(@标题栏_粗加工)"sv },
					{ LR"(@标题栏文字(1).dwg)"s,LR"(@标题栏文字(1))"sv },
			};
			return { varData,ArraySize<decltype(varData)>::value() };
		}
		inline void ThisFunction::_p_updateBlockSearchDir() {
			if ($BlockSearchDir.empty()) {
				{
					if (QFileInfo::exists(VQLS(R"(E:/Duty/Duty/template/blocks)"))) {
						$BlockSearchDir = LR"(E:\Duty\Duty\template\blocks\)"s;
						return;
					}
				}
				QtApplication qapp;
				QString varDir = QFileDialog::getExistingDirectory();
				if (varDir.isEmpty()) { return; }
				if (varDir.endsWith(QChar('/')) || varDir.endsWith(QChar('\\'))) {
					$BlockSearchDir = varDir.toStdWString();
				}
				else {
					varDir += '/';
					$BlockSearchDir = varDir.toStdWString();
				}
			}
		}

		inline void  ThisFunction::update() {
			Acad::ErrorStatus varAError;
			_p_updateBlockSearchDir();
			auto varFiles = _p_getBlockFiles();
			const auto varBegin = varFiles.first;
			const auto varEnd = varBegin + varFiles.second;
			for (auto varPos = varBegin; varPos != varEnd; ++varPos) {
				const auto varOtherFileName = $BlockSearchDir + varPos->first;
				const auto & varBlockName = varPos->second;

				std::unique_ptr<AcDbDatabase> varOtherFile{ new AcDbDatabase{ false } };
				if (Acad::eOk != varOtherFile->readDwgFile(varOtherFileName.c_str())) {
					acutPrintf(LR"(打开文件")");
					acutPrintf(varOtherFileName.c_str());
					acutPrintf(LR"("失败
)");
					continue;
				}

				{

					AcDbObjectId varBlockID;
					if ((varAError = $TargetDataBase->insert(varBlockID,
						varBlockName.data(),
						varOtherFile.get(),
						false)) != Acad::eOk) {
						acutPrintf(LR"(插入块失败:)");
						acutPrintf(acadErrorStatusText(varAError));
						acutPrintf(LR"(
)");
						continue;
					}
				}

			}
		}

	}/*namespace*/

	void UpdateBlockFromOtherFile::main() {
		ThisFunction varThis(
			acdbHostApplicationServices()->workingDatabase());
		varThis.update();
	}/*UpdateBlockFromOtherFile::main*/

	UpdateBlockFromOtherFile::UpdateBlockFromOtherFile() {

	}

}/*namespace sstd*/



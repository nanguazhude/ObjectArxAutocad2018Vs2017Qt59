#include "UpdateBlockFromOtherFile.hpp"
#include <string>
#include <string_view>
#include <set>
#include <tuple>
#include <dbAnnotativeObjectPE.h>

using namespace std::string_view_literals;
using namespace std::string_literals;

namespace sstd {

	extern void loadUpdateBlockFromOtherFile() { UpdateBlockFromOtherFile::load(); }

	void UpdateBlockFromOtherFile::load() {
		arx_add_main_command<UpdateBlockFromOtherFile>();
	}

	namespace {

		inline auto setAnnotative(
			AcDbObject* argObject,
			bool argIsAnnotative) {
			AcDbAnnotativeObjectPE * varAnnoPE = ACRX_PE_PTR(
				argObject,
				AcDbAnnotativeObjectPE);
			if (varAnnoPE == nullptr) {
				return Acad::eNullPtr;
			}

			if (varAnnoPE->annotative(argObject) == argIsAnnotative) {
				return Acad::eOk;
			}

			return varAnnoPE->setAnnotative(argObject, argIsAnnotative);
		}

		inline auto setAnnotative(AcDbObjectId argID, bool argIsAnnotative) {
			sstd::ArxClosePointer< AcDbObject > varObject;
			if (eOk != acdbOpenObject(varObject.pointer(), argID, AcDb::kForWrite)) {
				return Acad::eNullPtr;
			}
			return setAnnotative(varObject.pointer(), argIsAnnotative);
		}

		using element_t = std::tuple<
			std::wstring/*filename*/,
			std::wstring_view/*blockname*/,
			bool/*if have the block then ignore it*/,
			bool/*注释性*/
		>;
		class ThisFunction final {
		public:
			std::set< std::wstring, std::less<> /**/> $BlockTables;
			AcDbDatabase * const $TargetDataBase;
			static std::wstring $BlockSearchDir;
			using BlockFiles = std::pair< const element_t *,
				int >;
			inline void update();
			ThisFunction(AcDbDatabase * arg) :$TargetDataBase(arg) {
				sstd::ArxClosePointer< AcDbBlockTable > varBlockTable;
				if (eOk != arg->getBlockTable(varBlockTable.pointer(), AcDb::kForRead)) {
					svthrow(LR"(can not open blcktable)");
				}
				std::unique_ptr< AcDbBlockTableIterator > varIt;
				{
					AcDbBlockTableIterator* varItem = nullptr;
					if (eOk != varBlockTable->newIterator(varItem)) { svthrow(LR"(null ptr)"); }
					if (varItem == nullptr) { svthrow(LR"(null ptr)"); }
					varIt.reset(varItem);
				}
				for (; !varIt->done(); varIt->step()) {
					sstd::ArxClosePointer<AcDbBlockTableRecord> varData;
					if (eOk != varIt->getRecord(varData.pointer()))continue;
					AcString varName;
					varData->getName(varName);
					$BlockTables.emplace(static_cast<const wchar_t *>(varName), varName.length());
				}

			}
			inline bool isTargetHasBlock(const wchar_t * arg);
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
			static const element_t varData[] = {
					{ LR"(_MY_ROW.dwg)"s,LR"(_MY_ROW)"sv ,false , false  }/*this must be the first*/,
					{ LR"(横边框2(G3000).dwg)"s,LR"(横边框2(G3000))"sv,false ,false  },
					{ LR"(_毂形位公差.dwg)"s,LR"(@毂形位公差.111133)"sv,false ,true },
					{ LR"(_刹车面形位公差.dwg)"s,LR"(@刹车面形位公差.111133)"sv,false , true },
					{ LR"(@标题栏_粗加工.dwg)"s,LR"(@标题栏_粗加工)"sv ,false ,false },
					{ LR"(@标题栏文字(1).dwg)"s,LR"(@标题栏文字(1))"sv ,true ,false },
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
					else if (QFileInfo::exists(VQLS(R"(C:/ProgramFiles/autocad_template/blocks)"))) {
						$BlockSearchDir = LR"(C:\ProgramFiles\autocad_template\blocks\)"s;
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

		inline bool ThisFunction::isTargetHasBlock(const wchar_t * arg) {
			return $BlockTables.count(std::wstring_view(arg)) > 0;
		}

		inline void  ThisFunction::update() {
			Acad::ErrorStatus varAError;
			_p_updateBlockSearchDir();
			auto varFiles = _p_getBlockFiles();
			const auto varBegin = varFiles.first;
			const auto varEnd = varBegin + varFiles.second;
			for (auto varPos = varBegin; varPos != varEnd; ++varPos) {
				const auto varOtherFileName = $BlockSearchDir + std::get<0>(*varPos);
				const auto & varBlockName = std::get<1>(*varPos);

				/*如果已经有此块，尝试是否跳过*/
				if (std::get<2>(*varPos)) {
					if (isTargetHasBlock(varBlockName.data())) {
						continue;
					}
				}

				std::unique_ptr<AcDbDatabase> varOtherFile{ new AcDbDatabase{ false } };
				/*bool annotativeDwg() const;
				Description
				Controls whether the drawing is treated as 
				an annotative block when it is inserted into other drawings.
				True to treat the drawing as an annotative block when 
				it is inserted into other drawings, false otherwise. 
				The default value for new databases is false.
				*/
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
					/*设置注释性*/
					setAnnotative(varBlockID, std::get<3>(*varPos));
				}

			}
		}

	}/*namespace*/

	void UpdateBlockFromOtherFile::main() try {
		ThisFunction varThis(
			acdbHostApplicationServices()->workingDatabase());
		varThis.update();
	}/*UpdateBlockFromOtherFile::main*/
	catch (...) {}

	UpdateBlockFromOtherFile::UpdateBlockFromOtherFile() {

	}

}/*namespace sstd*/



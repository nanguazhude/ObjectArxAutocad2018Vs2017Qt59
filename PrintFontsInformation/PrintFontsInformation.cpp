#include <object_arx_global.hpp>
#include "PrintFontsInformation.hpp"

namespace sstd {

	extern void loadPrintFontsInformation() { PrintFontsInformation::load(); }

	PrintFontsInformation::PrintFontsInformation() {
	}

	void PrintFontsInformation::load() {
		arx_add_main_command<PrintFontsInformation>();
	}

	void PrintFontsInformation::main() {
		auto varDB = acdbHostApplicationServices()
			->workingDatabase();
		sstd::ArxClosePointer<AcDbTextStyleTable> argTST;
		if (eOk != varDB->getTextStyleTable(argTST.pointer())) {
			return;
		}
		std::unique_ptr<AcDbTextStyleTableIterator> varIt;
		{
			AcDbTextStyleTableIterator * varTmp = nullptr;
			if (eOk != argTST->newIterator(varTmp)) {
				return;
			}
			varIt.reset(varTmp);
		}
		for (; !varIt->done(); varIt->step()) {
			sstd::ArxClosePointer< AcDbTextStyleTableRecord > varItem;
			if (eOk != varIt->getRecord(varItem.pointer())) {
				continue;
			}
			wchar_t * varTmp = nullptr;
			constexpr auto SNullptr = LR"(nullptr)";
			{
				varItem->getName(varTmp);
				sstd::ArxString varN{ varTmp };
				varTmp ? acutPrintf(varTmp) : acutPrintf(SNullptr);
				acutPrintf(LR"(
)");
			}
			{
				varTmp = nullptr;
				varItem->fileName(varTmp);
				sstd::ArxString varN{ varTmp };
				varTmp ? acutPrintf(varTmp) : acutPrintf(SNullptr);
				acutPrintf(LR"(
)");
			}
			{
				varTmp = nullptr;
				varItem->bigFontFileName(varTmp);
				sstd::ArxString varN{ varTmp };
				varTmp ? acutPrintf(varTmp) : acutPrintf(SNullptr);
				acutPrintf(LR"(
)");
			}
			{
				wchar_t * pTypeface = nullptr;
				bool bold = false;
				bool italic = false;
				Charset charset;/*kChineseSimpCharset*/
				Autodesk::AutoCAD::PAL::FontUtils::FontPitch pitch;/*kFixed*/
				Autodesk::AutoCAD::PAL::FontUtils::FontFamily family;/*kModern*/
				varItem->font(pTypeface,bold,italic,charset,pitch,family);
				sstd::ArxString varN{ pTypeface };
				acutPrintf(LR"(Font:
)");
				acutPrintf( pTypeface );
				acutPrintf(LR"(
)");
				acutPrintf( get_cstr(std::to_wstring( static_cast<int>(charset))) );
				acutPrintf(LR"(
)"); 
				acutPrintf(get_cstr(std::to_wstring(static_cast<int>(pitch))));
				acutPrintf(LR"(
)");
				acutPrintf(get_cstr(std::to_wstring(static_cast<int>(family))));
				acutPrintf(LR"(
)");
			}
			acutPrintf(LR"(:---------:
)");
		}
	}

}/*namespace sstd*/

/**
ACDBCORE2D_PORT Acad::ErrorStatus font   (ACHAR*& pTypeface,
Adesk::Boolean& bold,
Adesk::Boolean& italic,
Charset& charset,
Autodesk::AutoCAD::PAL::FontUtils::FontPitch& pitch,
Autodesk::AutoCAD::PAL::FontUtils::FontFamily& family) const;
protected:
**/


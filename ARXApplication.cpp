#include "object_arx_global.hpp"
#include "ARXApplication.hpp"
#include <array>
#include <thread>
#include <string_view>

using namespace std::string_view_literals;

namespace sstd {

	extern void loadHellowWord();
	extern void loadSimpleDrawLine();
	extern void loadSimpleDrawLineChangeColor();
	extern void loadSimpleDrawPolygon();
	extern void loadPrintALlLayerName();
	extern void loadUpdateBlockFromOtherFile();
	extern void loadTestCode();
	extern void loadUpdateLayer();
	extern void loadUpdateTextStyle();
	extern void loadUpdateDimStyle();
	extern void loadUpdateReactor();
	extern void loadAddMaskToMleader();
	extern void loadUpdateMLeaderStyle();
	extern void loadUpdateAll();
	extern void loadUpdatePart();
	extern void loadAddMaskToMText();
	extern void loadUpdateTitleBlockTime();
	extern void loadHideDimEdge();
	extern void loadCopyFilePathToClipboard();
	extern void loadPrintFontsInformation();
	extern void loadDrawMultiCircles();
	extern void loadDrawMRectangle();
	extern void loadDrawMStep();
	extern void loadFARC();
	extern void loadSimpleSelect();
	extern void loadFRect();
	extern void loadFVAngleLine();
	extern void loadFTRect();
	extern void loadFCTRect();
	extern void loadHideLayer();
	extern void loadMakeDimStyle();
	extern void loadEChamfer();
	extern void loadEFillet();
	extern void loadArxZCC();
	extern void loadFConnect();
    extern void loadEText();
    extern void loadDimDistance();
	extern void loadEPrint();
	extern void loadEScale();
	extern void loadEResetAnnotationScale();
	extern void loadFEraseAll();
	extern void loadConvertDA();
	extern void loadDrawCrossLine();
	extern void loadWrapDim();

	namespace {
		using FunctionType = void(*)(void);
		static constexpr FunctionType _v_functions[] = {
			{ &sstd::loadHellowWord },
			{ &sstd::loadSimpleDrawLine },
			{ &sstd::loadSimpleDrawLineChangeColor },
			{ &sstd::loadSimpleDrawPolygon },
			{ &sstd::loadPrintALlLayerName },
			{ &sstd::loadUpdateBlockFromOtherFile },
			{ &sstd::loadTestCode },
			{ &sstd::loadUpdateLayer },
			{ &sstd::loadUpdateTextStyle },
			{ &sstd::loadUpdateDimStyle },
			{ &sstd::loadUpdateReactor },
			{ &sstd::loadAddMaskToMleader },
			{ &sstd::loadUpdateMLeaderStyle },
			{ &sstd::loadUpdateAll },
			{ &sstd::loadUpdatePart },
			{ &sstd::loadAddMaskToMText },
			{ &sstd::loadUpdateTitleBlockTime },
			{ &sstd::loadHideDimEdge },
			{ &sstd::loadCopyFilePathToClipboard },
			{ &sstd::loadPrintFontsInformation },
			{ &sstd::loadDrawMultiCircles },
			{ &sstd::loadDrawMRectangle },
			{ &sstd::loadDrawMStep },
			{ &sstd::loadFARC },
			{ &sstd::loadSimpleSelect },
			{ &sstd::loadFRect },
			{ &sstd::loadFVAngleLine },
			{ &sstd::loadFTRect },
			{ &sstd::loadFCTRect },
			{ &sstd::loadHideLayer },
			{ &sstd::loadMakeDimStyle },
			{ &sstd::loadEChamfer },
			{ &sstd::loadEFillet },
			{ &sstd::loadArxZCC },
			{ &sstd::loadFConnect },
            { &sstd::loadEText },
            { &sstd::loadDimDistance },
		    { &sstd::loadEPrint },
		    { &sstd::loadEScale },
		    { &sstd::loadEResetAnnotationScale },
		    { &sstd::loadFEraseAll },
		    { &sstd::loadConvertDA },
		    { &sstd::loadDrawCrossLine },
		    { &sstd::loadWrapDim },
		};
	}

	void ARXApplication::load() {
		for (const auto & varI : _v_functions) {
			(varI)();
		}
	}

	void ARXApplication::unload() {
		acedRegCmds->removeGroup(arx_group_name());
	}

	ARXApplication::ARXApplication() {

	}

}/*namespace sstd*/

/********************************/

namespace sstd {

	extern void WCS2DCS(const double * i,double * o) {
		const static class V {
		public:
			struct resbuf wcs, dcs;
			V() {
				wcs.rbnext = nullptr;
				wcs.restype = RTSHORT;
				wcs.resval.rint = 0;

				dcs.rbnext = nullptr;
				dcs.restype = RTSHORT;
				dcs.resval.rint = 2;
			}
		}var;

		ads_point pt1;
		pt1[0] = *i++;
		pt1[1] = *i++;
		pt1[2] = *i++;

		acedTrans(pt1, &var.wcs, &var.dcs, 0, o);

	}

	extern void UCS2WCS(const double * i, double *o) {

		const static class V {
		public:
			struct resbuf wcs, ucs;
			V() {
				wcs.rbnext = nullptr;
				wcs.restype = RTSHORT;
				wcs.resval.rint = 0;

				ucs.rbnext = nullptr;
				ucs.restype = RTSHORT;
				ucs.resval.rint = 1;
			}
		}var;

		ads_point pt1;
		pt1[0] = *i++;
		pt1[1] = *i++;
		pt1[2] = *i++;

		acedTrans(pt1, &var.ucs, &var.wcs, 0, o);

	}
}/*sstd*/

/************************************************/
#include "ThirdPart/part_google_v8/include/double-conversion/double-conversion.h"
namespace sstd {
	namespace {
		class _StaticData_int_double_to_string_ {
		public:
			constexpr const static int N = 1024;
			std::array<char, N> char_chache;
			std::array<wchar_t, N> wchar_chache;

			std::wstring_view int_to_string(int data) {
				/*加速运算结果*/
				switch (data) {
				case 0: return  LR"(0)"sv;
				case 1: return  LR"(1)"sv;
				case 2: return  LR"(2)"sv;
				case 3: return  LR"(3)"sv;
				case 4: return  LR"(4)"sv;
				case 5: return  LR"(5)"sv;
				case 6: return  LR"(6)"sv;
				case 7: return  LR"(7)"sv;
				case 8: return  LR"(8)"sv;
				case 9: return  LR"(9)"sv;
				case 10: return LR"(10)"sv;
				}

				auto varPointerEnd = &(*wchar_chache.rbegin());
				auto varPointer = varPointerEnd;
				const bool isN = data < 0 ? (data = -data, true) : false;

				{
					lldiv_t varTmp;
					varTmp.quot = data;
					do {
						varTmp = std::lldiv(varTmp.quot, 10);
						switch (varTmp.rem) {
						case 0:*varPointer = L'0'; break;
						case 1:*varPointer = L'1'; break;
						case 2:*varPointer = L'2'; break;
						case 3:*varPointer = L'3'; break;
						case 4:*varPointer = L'4'; break;
						case 5:*varPointer = L'5'; break;
						case 6:*varPointer = L'6'; break;
						case 7:*varPointer = L'7'; break;
						case 8:*varPointer = L'8'; break;
						case 9:*varPointer = L'9'; break;
						}
						--varPointer;
					} while (varTmp.quot);
				}

				if (isN) {
					*varPointer = L'-';
					--varPointer;
				}

				return { varPointer + 1, static_cast<std::size_t>( varPointerEnd - varPointer ) };
			}

			std::wstring_view double_to_string(double a) try{
			 
				double_conversion::StringBuilder varSB{ char_chache.data(),N };
				double_conversion::DoubleToStringConverter::EcmaScriptConverter().ToShortest(a, &varSB);
				 
				const auto n = varSB.position();
				{
					auto wi = wchar_chache.begin();
					auto ci = char_chache.cbegin();
					for (int i = 0; i < n; ++i) {
						*wi = *ci;
						++wi; ++ci;
					}
					*wi = 0;
				}
				 				
				return std::wstring_view{ wchar_chache.data(),static_cast<std::size_t>(n) };

			}
			catch (...) {
				acutPrintf(LR"(there is exception when cv double to string
)");
				return{};
			}

			static _StaticData_int_double_to_string_ & instance() {
				static _StaticData_int_double_to_string_ ans;
				return ans;
			}

		};
		 
	}
	
	extern std::wstring_view double_to_string(double a) {
		return _StaticData_int_double_to_string_::instance().double_to_string(a);
	}
	extern std::wstring_view int_to_string(int a) {
		return _StaticData_int_double_to_string_::instance().int_to_string(a);
	}
	
}/*namespace sstd*/
/************************************************/


#include "object_arx_global.hpp"
#include "ARXApplication.hpp"
#include <thread>

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
		    { &sstd::loadHideLayer }
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

	extern void UCS2WCS(const double * i, double *o) {

		static class V {
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


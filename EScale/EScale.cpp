#include "EScale.hpp"
#include <dbAnnotationScale.h>
#include <dbObjectContextManager.h>
#include <dbObjectContextInterface.h>
#include <dbObjectContextCollection.h>

namespace sstd {
	extern void loadEScale() {
		ECalse::load();
	}
}/*sstd*/

void sstd::ECalse::main() try{
	AcString varName;
	auto DB = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectContextManager* contextMgr = DB->objectContextManager();
	AcDbObjectContextCollection* contextCollection =
		contextMgr->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

	if (!contextCollection->hasContext(LR"(E)")){
		// basic scale object on stack, to add
		AcDbAnnotationScale annoScaleToAdd;
		auto acadErr = annoScaleToAdd.setName(varName);
		acadErr = annoScaleToAdd.setPaperUnits(260.);
		acadErr = annoScaleToAdd.setDrawingUnits(260.);
		// add annotation scale to overall database contexts
		acadErr = contextCollection->addContext(&annoScaleToAdd);
	}
	else {
	}

}
catch (...) {}

void sstd::ECalse::load() {
	arx_add_main_command<ECalse>();
}




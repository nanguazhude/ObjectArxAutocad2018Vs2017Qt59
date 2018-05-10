#include "EScale.hpp"
#include <dbAnnotationScale.h>
#include <dbObjectContextManager.h>
#include <dbObjectContextInterface.h>
#include <dbObjectContextCollection.h>
#include <QtQml/QtQml>

namespace sstd {
	extern void loadEScale() {
		ECalse::load();
	}
	extern void update_dim_distance(double x);
}/*sstd*/

void sstd::ECalse::main() try {
	struct LockThisMain {
		~LockThisMain() {
			auto varCD = acDocManager->curDocument();
			if (varCD) {
				acDocManager->sendStringToExecute(
					varCD,
					LR"((setvar "HPANNOTATIVE" 1)
(setvar "HPLAYER" "细实线")
(setvar "HPNAME" "ANSI31")
(setvar "HPASSOC" 1)
)");
			}
		}
	}varLockThisMain;

	const AcString varName = LR"(E)";
	double varKeyValue = 0;

	auto newKeyValue = [&varKeyValue] {
		AcString varNumberString;
		/*如果函数调用成功返回RTNORM，如果失败返回RTERROR，如果用户取消(按Esc键)则返回RTCAN。*/
		if (RTNORM != acedGetString(true, LR"(输入关键尺寸：)", varNumberString)) {
			svthrow(LR"(获得关键尺寸失败！)");
		}
		QtApplication varQtApplication;
		QJSEngine varJSE;
		auto varAns = varJSE.evaluate(QString::fromWCharArray(varNumberString.constPtr()));
		if (varAns.isError()) {
			svthrow(LR"(输入有误！)");
		}
		varKeyValue = varAns.toNumber();
		if (varKeyValue > 0) {
			return;
		}
		else {
			svthrow(LR"(输入有误！)");
		}
	};

	auto DB = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectContextManager* contextMgr = DB->objectContextManager();
	AcDbObjectContextCollection* contextCollection =
		contextMgr->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

	if (!contextCollection->hasContext(varName)) {
		newKeyValue();
		{
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varName);
			acadErr = annoScaleToAdd.setPaperUnits(260.);
			acadErr = annoScaleToAdd.setDrawingUnits(varKeyValue);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
	}

	double varPU = 1;
	double varDU = 1;
	{
		auto varTmp = contextCollection->getContext(varName);
		auto varScale = dynamic_cast<AcDbAnnotationScale*>(varTmp);
		if (varScale == nullptr) {
			delete varTmp;
			svthrow(LR"(logical error!)");
		}
		if (varScale) {
			DB->setCannoscale(varScale);
			varScale->getPaperUnits(varPU);
			varScale->getDrawingUnits(varDU);
			varKeyValue = varDU / varPU;
		}
		delete varScale;
	}

	{
		auto varTmpName = varName + LR"(P80)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*0.8);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P60)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*0.6);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P90)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*0.9);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P70)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*0.7);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P110)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*1.1);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P120)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*1.2);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P130)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*1.3);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
		varTmpName = varName + LR"(P140)";
		if (!contextCollection->hasContext(varTmpName)) {
			// basic scale object on stack, to add
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(varTmpName);
			acadErr = annoScaleToAdd.setPaperUnits(varPU);
			acadErr = annoScaleToAdd.setDrawingUnits(varDU*1.4);
			// add annotation scale to overall database contexts
			acadErr = contextCollection->addContext(&annoScaleToAdd);
		}
	}

	if (varKeyValue > 0)update_dim_distance(varKeyValue);

}
catch (...) {}

void sstd::ECalse::load() {
	arx_add_main_command<ECalse>();
}




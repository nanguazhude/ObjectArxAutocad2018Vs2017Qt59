#include "EResetAnnotationScale.hpp"
#include <acedCmdNF.h>
#include <dbAnnotationScale.h>
#include <dbObjectContextManager.h>
#include <dbObjectContextInterface.h>
#include <dbObjectContextCollection.h>
#include <dbAnnotativeObjectPE.h>
#include <string>
#include <memory>
#include <set>
#include <optional>
#include <string_view>

namespace sstd {
	extern void loadEResetAnnotationScale() {
		EResetAnnotationScale::load();
	}
}/*namespace sstd*/

void sstd::EResetAnnotationScale::load() {
	arx_add_main_command<EResetAnnotationScale>();
}

namespace {
	namespace thisfile {
		using SetAcDbAnnotationScale = std::set< std::unique_ptr<AcDbAnnotationScale> /**/>;
		constexpr auto one_by_one_name = LR"(1:1)";
		/**/
		void reset_item(AcDbEntity *  varEnt/*当前对象*/,
			AcDbAnnotationScale * varScaleOneOne/*必须增加的元素*/,
			const SetAcDbAnnotationScale & varAboutToDelete/*要删除的元素*/) {
			AcDbAnnotativeObjectPE * varANN = ACRX_PE_PTR(varEnt, AcDbAnnotativeObjectPE);
			if (varANN == nullptr) { return; }
			/*如果这不是一个注释对象,则结束*/
			if (varANN->annotative(varEnt) == false) { return; }
			/*获得interface*/
			AcDbObjectContextInterface *varContextInterface = ACRX_PE_PTR(varEnt, AcDbObjectContextInterface);
			/*增加元素*/
			if (!varContextInterface->hasContext(varEnt, *varScaleOneOne)) {
				varContextInterface->addContext(varEnt, *varScaleOneOne);
			}

			/*删除元素*/
			for (const auto & varI : varAboutToDelete) {
				if (!varContextInterface->hasContext(varEnt, *varI)) {
					continue;
				}
				varContextInterface->removeContext(varEnt, *varI);
			}

		}/*void reset_item(AcDbEntity * arg)*/
	}/*namespace thisfile*/
}/*namespace*/

void sstd::EResetAnnotationScale::main() try {
	acutPrintf(LR"(void sstd::EResetAnnotationScale::main())");
	/*遍历所有对象*/
	auto $DB = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectContextManager *varManager = acdbCurDwg()->objectContextManager();
	if (varManager == nullptr) {
		svthrow(LR"(varManager == nullptr)");
		return;
	}
	/*获得scalelist*/
	AcDbObjectContextCollection *varContextCollection = varManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);
	if (varContextCollection == nullptr) {
		svthrow(LR"(varContextCollection == nullptr)");
		return;
	}
	AcDbAnnotationScale * varScaleOneOne = nullptr;
	/*try add 1:1 , add set varScaleOneOne value */
	{
		if (!varContextCollection->hasContext(thisfile::one_by_one_name)) {
			AcDbAnnotationScale annoScaleToAdd;
			auto acadErr = annoScaleToAdd.setName(thisfile::one_by_one_name);
			acadErr = annoScaleToAdd.setPaperUnits(1.);
			acadErr = annoScaleToAdd.setDrawingUnits(1.);
			// add annotation scale to overall database contexts
			acadErr = varContextCollection->addContext(&annoScaleToAdd);
		}
		varScaleOneOne = dynamic_cast<AcDbAnnotationScale*>(
			varContextCollection->getContext(thisfile::one_by_one_name));
	}
	if (varScaleOneOne == nullptr) {
		svthrow(LR"(varScaleOneOne == nullptr)");
		return;
	}
	$DB->setCannoscale(varScaleOneOne);
	thisfile::SetAcDbAnnotationScale varAboutToRemove;
	{
		std::unique_ptr<AcDbObjectContextCollectionIterator> varIt;
		{
			auto var = varContextCollection->newIterator();
			if (var == nullptr) { return; }
			varIt.reset(var);
		}
		for (varIt->start(); !varIt->done(); varIt->next()) {
			std::unique_ptr<AcDbAnnotationScale> var  ;
			{
				AcDbObjectContext * tmp = nullptr;
				varIt->getContext(tmp);
				if (tmp == nullptr) { continue; }
				auto var1 = dynamic_cast<AcDbAnnotationScale *>(tmp);
				if (var1 == nullptr) { delete tmp; continue; }
				var.reset(var1);
			}
			AcString varName;
			var->getName(varName);
			if (varName == thisfile::one_by_one_name) { continue; }
			varAboutToRemove.insert(std::move(var));
		}
	}
	if (varAboutToRemove.empty()) { return; }
	sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
	if (Acad::eOk == $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
		std::unique_ptr<AcDbBlockTableIterator> varIt;
		{
			AcDbBlockTableIterator * varTmp = nullptr;
			if (Acad::eOk != varBlockTable->newIterator(varTmp)) {
				return;
			}
			varIt.reset(varTmp);
		}

		for (; !varIt->done(); varIt->step()) {
			sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
			if (Acad::eOk != varIt->getRecord(varR)) {
				continue;
			}
			/********************************************/
			{
				auto varT = varR.pointer();
				std::unique_ptr<AcDbBlockTableRecordIterator> varIt;
				{
					AcDbBlockTableRecordIterator *varTmp;
					if (varT->newIterator(varTmp) != Acad::eOk) {
						return;
					}
					varIt.reset(varTmp);
				}

				for (; !varIt->done(); varIt->step()) {
					sstd::ArxClosePointer< AcDbEntity> varEnt;
					if (varIt->getEntity(varEnt, AcDb::kForWrite) != Acad::eOk) {
						continue;
					}

					try {
						thisfile::reset_item(varEnt.pointer(),
							varScaleOneOne,
							varAboutToRemove);
					}
					catch (...) {
						/*ignore all exceptions*/
					}

				}
			}
			/********************************************/
		}

	}

}
catch (...) {
}

sstd::EResetAnnotationScale::EResetAnnotationScale() {
}






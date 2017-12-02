#include <algorithm>
#include "GetAllLayerName.hpp"

namespace sstd {

	extern void loadPrintALlLayerName() { GetAllLayerName::load(); }

	GetAllLayerName::GetAllLayerName() {

	}

	auto GetAllLayerName::getAllLayerName()->GetAllLayerName::ReturnType {
		ReturnType varAns= std::make_shared<ReturnType::element_type>();
		varAns->reserve(128);

		sstd::ArxClosePointer< AcDbLayerTable > varLayerTable;

		{
			auto varAHAS = acdbHostApplicationServices();
			if (nullptr == varAHAS) { return {}; }
			auto varWDB = varAHAS->workingDatabase();
			if (nullptr == varWDB) { return {}; }
			varWDB->getLayerTable(varLayerTable,AcDb::kForRead);
			if (bool(varLayerTable) == false) { return {}; }
		}

		std::unique_ptr<AcDbLayerTableIterator> varIt;
		{
			AcDbLayerTableIterator * varItTmp=nullptr;
			if (Acad::eOk != varLayerTable->newIterator(varItTmp)) {
				return{};
			}
			varIt.reset(varItTmp);
		}
		
		for (varIt->start(); !varIt->done();varIt->step()) {
			sstd::ArxClosePointer< AcDbLayerTableRecord > varLTR;
			if (Acad::eOk == varIt->getRecord(varLTR,AcDb::kForRead)) {
				wchar_t * varName = nullptr;
				wchar_t * varName1 = nullptr;
				AcDbLinetypeTableRecord * varLinetypeTRCD=nullptr;
				acdbOpenObject(varLinetypeTRCD,varLTR->linetypeObjectId(),
					AcDb::kForRead);
				varLinetypeTRCD->getName(varName1);
				if (Acad::eOk == varLTR->getName(varName)) {
					varAns->emplace_back(std::wstring(varName) +LR"(    )" + varName1);
				}
				varLTR->linetypeObjectId();
			}
		}

		/*sort alllayer name*/
		std::sort(varAns->begin(), varAns->end(), [](const auto & l,const auto &r) {
			const auto ls = l.size();
			const auto rs = r.size();
			if (ls < rs) { return true; }
			if (ls > rs) { return false; }
			return l < r;
		});
		/*free memory not used*/
		varAns->shrink_to_fit();

		return std::move(varAns);
	}

	void GetAllLayerName::load() {
		arx_add_main_command<GetAllLayerName>();
	}

	void GetAllLayerName::main() {
		const auto varAllLayerName = getAllLayerName();
		if (varAllLayerName) {
			for (const auto & varI : (*varAllLayerName)) {
				acutPrintf( varI.c_str() );
				acutPrintf(LR"(
)");
			}
		}
	}

}/*namespace sstd*/


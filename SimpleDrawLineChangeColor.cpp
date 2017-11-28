#include "SimpleDrawLineChangeColor.hpp"

namespace sstd {

	extern void loadSimpleDrawLineChangeColor() { SimpleDrawLineChangeColor::load(); }

	SimpleDrawLineChangeColor::SimpleDrawLineChangeColor() {

	}

	void SimpleDrawLineChangeColor::load() {
		arx_add_main_command<SimpleDrawLineChangeColor>();
	}

	inline AcDbObjectId simpleDrawLine() {
		const AcGePoint3d varStart{ 0,0,0 };
		const AcGePoint3d varEnd{ 100,100,0 };
		sstd::ArxClosePointer<AcDbLine> varLine{ new AcDbLine{ varStart,varEnd } };

		sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
		sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

		acdbHostApplicationServices()->workingDatabase()
			->getBlockTable(varBlockTable, AcDb::kForRead);
		if (varBlockTable) {
			varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord,
				AcDb::kForWrite);
			if (varBlockTableRecord) {
				AcDbObjectId varLineID;
				varBlockTableRecord->appendAcDbEntity(varLineID, varLine);
				return varLineID;
			}
		}
		return{};
	}

	void SimpleDrawLineChangeColor::main() {
		AcDbObjectId varLineID = simpleDrawLine();
		if (varLineID.isNull()) { return; }
		constexpr const Adesk::UInt16 varColorIndex = 1;
		sstd::ArxClosePointer<AcDbEntity> varEntity;
		sstd::openObject(varEntity, varLineID, AcDb::kForWrite);
		if (varEntity) {
			varEntity->setColorIndex(varColorIndex);
		}
	}

}/*namespace sstd*/

/********************************/

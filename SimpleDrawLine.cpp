#include "SimpleDrawLine.hpp"

namespace sstd {

	SimpleDrawLine::SimpleDrawLine() {

	}

	void SimpleDrawLine::load() {
        arx_add_main_command<SimpleDrawLine>();
	}

	void SimpleDrawLine::main() {
		const AcGePoint3d varStart{0,0,0};
		const AcGePoint3d varEnd{100,100,0};
		sstd::ArxClosePointer<AcDbLine> varLine{ new AcDbLine{varStart,varEnd} };

		sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
		sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

		acdbHostApplicationServices()->workingDatabase()
			->getBlockTable(varBlockTable,AcDb::kForRead);
		varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, AcDb::kForWrite);

		if (varBlockTable&&varBlockTableRecord) {
			AcDbObjectId varLineID;
			varBlockTableRecord->appendAcDbEntity(varLineID,varLine);
		}
	}

}/*namespace sstd*/

/********************************/


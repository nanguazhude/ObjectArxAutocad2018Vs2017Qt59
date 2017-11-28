#include "SimpleDrawLine.hpp"

namespace sstd {

	extern void loadSimpleDrawLine() { SimpleDrawLine::load(); }

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
		if (varBlockTable) {
			varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, 
				AcDb::kForWrite);
			if (varBlockTableRecord) {
				AcDbObjectId varLineID;
				acutPrintf(varLine->objectId().isNull() ? LR"(LineID is Null
)" : LR"(LineID is't Null
)");
				acutPrintf(varBlockTable->objectId().isNull() ? LR"(TableID is Null
)" : LR"(TableID is't Null
)");
				acutPrintf(varBlockTableRecord->objectId().isNull() ? LR"(TableRecordID is Null
)" : LR"(TableRecordID is't Null
)");
				varBlockTableRecord->appendAcDbEntity(varLineID, varLine);
				acutPrintf(varLine->objectId().isNull() ? LR"(LineID is Null
)" : LR"(LineID is't Null
)");
			}
		}
	}

}/*namespace sstd*/

/********************************/


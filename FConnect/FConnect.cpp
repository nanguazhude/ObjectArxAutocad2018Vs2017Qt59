#include "FConnect.hpp"

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {
	extern void loadFConnect() {
		FConnect::load();
	}
}/*namespace sstd*/

namespace sstd {

	FConnect::FConnect() {

	}

	void FConnect::load() {
		arx_add_main_command<FConnect>();
	}

	void FConnect::main() try {

		auto $DB = acdbHostApplicationServices()->workingDatabase();

		class ThisLock {
		public:
			AcDbDatabase * const $DB;
			struct resbuf $osmode;
			struct resbuf $snapmode;
			struct resbuf $autosnap;
			inline ThisLock(AcDbDatabase * argDB) :$DB(argDB) {
				const static class SetValue {
				public:
					struct resbuf $osmode/*1*/;
					struct resbuf $snapmode/*0*/;
					struct resbuf $autosnap/*31*/;
					SetValue() {
						$osmode.restype = RTSHORT;
						$snapmode.restype = RTSHORT;
						$autosnap.restype = RTSHORT;

						$osmode.rbnext = nullptr;
						$snapmode.rbnext = nullptr;
						$autosnap.rbnext = nullptr;

						$autosnap.resval.rint = 31;
						$snapmode.resval.rint = 0;
						$osmode.resval.rint = 1;
					}

					void setValue() const {
						acedSetVar(LR"(AUTOSNAP)", &$autosnap);
						acedSetVar(LR"(SNAPMODE)", &$snapmode);
						acedSetVar(LR"(OSMODE)", &$osmode);
					}

				} varSetValue;

				acedGetVar(LR"(AUTOSNAP)", &$autosnap);
				acedGetVar(LR"(SNAPMODE)", &$snapmode);
				acedGetVar(LR"(OSMODE)", &$osmode);
				varSetValue.setValue();

			}
			inline ~ThisLock() {
				acedSetVar(LR"(AUTOSNAP)", &$autosnap);
				acedSetVar(LR"(SNAPMODE)", &$snapmode);
				acedSetVar(LR"(OSMODE)", &$osmode);
			}
		}varThisLock{ $DB };

		AcGePoint3d varP1, varP2;
		/************************/
		auto varGetP1 = [&varP1]() {
			auto varError = acedGetPoint(nullptr, LR"(拾取第一个点:)", &varP1.x);
			if (RTNORM == varError) {
				varP1 = UcsToWorld(varP1);
				return;
			}
			throw varError;
		};
		/************************/
		auto varGetP2 = [&varP1, &varP2]() {
			auto varError = acedGetPoint(&varP1.x, LR"(拾取第二个点:)", &varP2.x);
			if (RTNORM == varError) {
				varP2 = UcsToWorld(varP2);
				return;
			}
			else if (RTNONE == varError) {
				/*使用上一次选择的点*/
				return;
			}
			throw varError;
		};
		/************************/
		sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
		sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
		if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) { throw 1; }
		if (eOk != varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, AcDb::kForWrite)) { throw 2; }
		/************************/
		auto varRunFunction = [&]() {
			varGetP1();
			varGetP2();
			if (varP1 == varP2) { return; }
			sstd::ArxClosePointer<AcDbLine> varLine{ new AcDbLine(varP1, varP2) };
			varBlockTableRecord->appendAcDbEntity(varLine);
			varLine->setLayer(LR"(粗实线)");
		};
		/************************/
		varRunFunction();
		for (;;) {
			varRunFunction();
		}

	}
	catch (...) {
		return;
	}

}/*namespace sstd*/


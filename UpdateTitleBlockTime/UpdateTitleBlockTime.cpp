#include <object_arx_global.hpp>
#include "UpdateTitleBlockTime.hpp"

namespace sstd {

	UpdateTitleBlockTime::UpdateTitleBlockTime() {
	}

	extern void loadUpdateTitleBlockTime() { UpdateTitleBlockTime::load(); }
	void UpdateTitleBlockTime::load() {
		arx_add_main_command<UpdateTitleBlockTime>();
	}

	class ThisFunction final {
	public:
		std::wstring $DataTime;
		AcDbDatabase * const $DB;
		inline void get_data_time();
		inline void set_date_time_to_block();
		ThisFunction(AcDbDatabase *arg) :$DB(arg) {}
	};

	inline void ThisFunction::get_data_time() {
		$DataTime.clear(); 
		const auto varCreateTime = $DB->tdcreate();

		auto toWstring = [](short arg) {
			return std::to_wstring(arg);
		};

		$DataTime = toWstring(varCreateTime.year());
		$DataTime += LR"(/)"sv;
		$DataTime += toWstring(varCreateTime.month());
		$DataTime += LR"(/)"sv;
		$DataTime += toWstring(varCreateTime.day());

		//acutPrintf( get_cstr($DataTime) );
	}

	inline void ThisFunction::set_date_time_to_block() {
		sstd::ArxClosePointer< AcDbBlockTable> varBlockTable;
		if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			return;
		}
		sstd::ArxClosePointer<AcDbBlockTableRecord>  varR;
		if (eOk != varBlockTable->getAt(LR"(横边框2(G3000))", 
			varR.pointer(),
			AcDb::kForRead)) {
			return;
		}

		std::unique_ptr<AcDbBlockReferenceIdIterator> varIt;
		{
			AcDbBlockReferenceIdIterator * varTmp;
			if (eOk != varR->newBlockReferenceIdIterator(varTmp)) { return; }
			varIt.reset(varTmp);
		}

		for (;!varIt->done();varIt->step()) {
			sstd::ArxClosePointer<AcDbBlockReference> varBR;
			if (eOk != varIt->getBlockReference(varBR, AcDb::kForWrite)) { 
				continue;
			}
			std::unique_ptr< AcDbObjectIterator> varAIt{ varBR->attributeIterator() };
			for (;!varAIt->done();varAIt->step()) {
				sstd::ArxClosePointer< AcDbAttribute > varAT;
				if (eOk != acdbOpenObject(varAT.pointer(), varAIt->objectId(), AcDb::kForWrite)) {
					continue;
				}
				auto varTag = varAT->tag();
				if (LR"(日期)"sv==varTag) {
					varAT->setTextString( get_cstr($DataTime)  );
				}
			}
		}

	}

	void UpdateTitleBlockTime::main() {
		ThisFunction varFunction( acdbHostApplicationServices()->workingDatabase() );
		varFunction.get_data_time();
		varFunction.set_date_time_to_block();
	}

}/*namespace sstd*/

/*************************************************/

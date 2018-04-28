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

		acutPrintf(LR"(日期 ：)");
		acutPrintf(get_cstr($DataTime));
		acutPrintf(LR"(
)");

	}

	inline void ThisFunction::set_date_time_to_block() {

		sstd::ArxClosePointer< AcDbBlockTable> varBlockTable;
		if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			acutPrintf(LR"(获得AcDbBlockTable失败)");
			acutPrintf(LR"(
)");
			return;
		}

		sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
		if (eOk != varBlockTable->getAt(LR"(横边框2(G3000))",
			varR.pointer(),
			AcDb::kForRead)) {
			acutPrintf(LR"(获得横边框2(G3000))");
			acutPrintf(LR"(
)");
			return;
		}

		AcDbObjectIdArray varIDS_;
		{
			AcDbDynBlockTableRecord varDR_{ varR->objectId() };
			if (varDR_.isDynamicBlock()) {
				AcDbObjectIdArray varBLKIDS_;
				//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR.close();
				//尝试从动态块获得ID
				varDR_.getAnonymousBlockIds(varBLKIDS_);
				//acutPrintf(L"---%d",varBLKIDS_.length());
				for (const auto & varJ : varBLKIDS_) {
					sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
					if (eOk != acdbOpenObject(varR.pointer(), varJ)) {
						continue;
					}
					AcDbObjectIdArray varTmpIDS;
					varR->getBlockReferenceIds(varTmpIDS);
					varIDS_.append(varTmpIDS);
				}
			}
			else {//尝试从普通块获得ID
				varR->getBlockReferenceIds(varIDS_);
				varR.close();
			}
		}

		std::ptrdiff_t varBlockCount = 0;
		for (const auto & varI : varIDS_) {/*for:1*/
			sstd::ArxClosePointer<AcDbBlockReference> varBR;
			if (eOk != acdbOpenObject(varBR.pointer(), varI, AcDb::kForWrite)) {
				acutPrintf(LR"(获得横边框2(G3000)对象迭代器失败)");
				acutPrintf(LR"(
)");
				continue;
			}

			++varBlockCount;
			std::unique_ptr< AcDbObjectIterator> varAIt{
				varBR->attributeIterator()
			};

			for (; !varAIt->done(); varAIt->step()) {/*for:2*/
				sstd::ArxClosePointer< AcDbAttribute > varAT;
				if (eOk != acdbOpenObject(varAT.pointer(),
					varAIt->objectId(),
					AcDb::kForWrite)) {
					acutPrintf(LR"(Can not open AcDbAttribute!
)");
					continue;
				}

				const sstd::ArxString varTag = varAT->tag();
				if (LR"(日期)"sv == varTag.pointer()) {
					varAT->setTextString(get_cstr($DataTime));

				}

			}/*for:2*/
		}/*for:1*/

		acutPrintf(LR"(共扫描：)");
		acutPrintf(get_cstr(std::to_wstring(varBlockCount)));
		acutPrintf(LR"(个数据
)");

	}

	void UpdateTitleBlockTime::main() {
		ThisFunction varFunction(acdbHostApplicationServices()->workingDatabase());
		varFunction.get_data_time();
		varFunction.set_date_time_to_block();
	}

}/*namespace sstd*/

/*************************************************/

#include <set>
#include <dbmleader.h>
#include "AddMaskToMleader.hpp"

namespace sstd {

	AddMaskToMleader::AddMaskToMleader() {

	}

	extern void loadAddMaskToMleader() { AddMaskToMleader::load(); }

	void AddMaskToMleader::load() {
		arx_add_main_command<AddMaskToMleader>();
	}

	namespace {
		class ThisCommandState {
		public:
			ThisCommandState(AcDbDatabase*arg) :$DB(arg) {}
			AcDbDatabase* $DB;
			std::set< AcDbObjectId > $SeletedItems;
			inline void makeSeleted();
			inline void addMask();
		protected:
			inline void _selete_a_block_table(AcDbBlockTableRecord*);
		};

		inline void ThisCommandState::makeSeleted() {

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
					_selete_a_block_table(varR);
				}

			}

		}

		inline void ThisCommandState::_selete_a_block_table(AcDbBlockTableRecord* varT) {
			std::unique_ptr<AcDbBlockTableRecordIterator> varIt;
			{
				AcDbBlockTableRecordIterator *varTmp;
				if (varT->newIterator(varTmp) != Acad::eOk) { return; }
				varIt.reset(varTmp);
			}

			for (; !varIt->done(); varIt->step()) {
				sstd::ArxClosePointer< AcDbEntity> varEnt;
				if (varIt->getEntity(varEnt, AcDb::kForRead) != Acad::eOk) {
					continue;
				}

				AcDbMLeader * varML = AcDbMLeader::cast(varEnt.pointer());
				if (varML) {
					if (AcDbMLeaderStyle::kMTextContent != varML->contentType()) {
						continue;
					}
					$SeletedItems.insert(varML->objectId());
				}

			}

		}

		inline void ThisCommandState::addMask() {
			for (const auto & varI : $SeletedItems) {
				sstd::ArxClosePointer< AcDbMLeader > varML;
				if (Acad::eOk != acdbOpenObject(varML.pointer(), varI, AcDb::kForWrite)) {
					continue;
				}

				std::unique_ptr<AcDbMText> varMtext{ varML->mtext() };
				{
					double varJ = 0;
					varMtext->getBackgroundScaleFactor(varJ);
					if ((varJ < 1.1) &&
						(varMtext->backgroundFillOn()) &&
						(varMtext->useBackgroundColorOn())) {
						continue;
					}
				}
				varMtext->setBackgroundFill(true);
				varMtext->setUseBackgroundColor(true);
				varMtext->setBackgroundScaleFactor(1.075);
				varML->setMText(varMtext.release());

			}
		}

	}

	void AddMaskToMleader::main() {

		ThisCommandState varThisCommand(
			acdbHostApplicationServices()->workingDatabase());
		varThisCommand.makeSeleted();
		varThisCommand.addMask();
	}

}/*namespace sstd*/



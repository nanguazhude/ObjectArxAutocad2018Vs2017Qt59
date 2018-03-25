#include "HideLayer.hpp"
#include <set>
#include <string_view>
#include "AcDbAssocGlobal.h"
using namespace std::string_view_literals;

namespace sstd {
	HideLayer::HideLayer() {

	}
}

namespace {
	class NamePair {
	public:
		std::wstring_view $LayerName;
		std::wstring_view $FunctionName;
	};
	constexpr NamePair globalNames[1] = {
		{LR"(粗实线)"sv,LR"(_wzt)"sv},
	};
}

namespace {

	class Pack {
	public:
		AcDbDatabase * $DB;
		std::set< AcDbObjectId > $SeletedItems;
		inline void makeSeleted();
		inline void hideObject();
		Pack();
	protected:
		inline void _selete_a_block_table(AcDbBlockTableRecord*);

	};

	inline void Pack::makeSeleted() {
		sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
		if (Acad::eOk == $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
			if (Acad::eOk != varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, AcDb::kForRead)) {
				return;
			}
			_selete_a_block_table(varBlockTableRecord);
		}
	}

	inline void Pack::_selete_a_block_table(AcDbBlockTableRecord*varT) {
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

			auto pvarLayerName = varEnt->layer();
			std::wstring_view varLayerName = pvarLayerName;
			if (varLayerName == LR"(粗实线)") {
				$SeletedItems.insert(varEnt->objectId());
			}
			acutDelString(pvarLayerName);

		}
	}

	inline void Pack::hideObject() {
		for (const auto & varI : $SeletedItems) {

			sstd::ArxClosePointer< AcDbEntity > varML;
			if (Acad::eOk != acdbOpenObject(varML.pointer(), varI, AcDb::kForWrite)) {
				continue;
			}

			varML->setVisibility(AcDb::kInvisible);

		}
	}

	Pack::Pack() {
		$DB = acdbHostApplicationServices()->workingDatabase();
	}

	template< std::size_t A >
	class AddName {
	public:
		inline static void add_name() {
			Pack pack;
			pack.makeSeleted();
			pack.hideObject();
		}
		inline static constexpr const wchar_t *globalName() { return globalNames[A].$FunctionName.data(); }
		inline static constexpr const wchar_t *localName() { return globalName() + 1; }
	};

	template<typename T>
	inline void _p_arx_add_main_command() {
		acedRegCmds->addCommand(sstd::arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_MODAL,
			&T::add_name
		);
	}

	extern void loadHideLayer() {
		_p_arx_add_main_command<AddName<0>>();
	}

}

/*************************************/





















#include "HideLayer.hpp"
#include <set>
#include <string_view>
#include "AcDbAssocGlobal.h"
#include <acedCmdNF.h>

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
		std::wstring_view $FunctionName1;
	};
	constexpr NamePair globalNames[] = {
	{ LR"(粗实线)"sv,LR"(_wzt)"sv,LR"(_qzt)"sv },
    { LR"(细实线)"sv,LR"(_wzf)"sv,LR"(_qzf)"sv },
	{ LR"(0)"sv,LR"(_wzz)"sv,LR"(_qzz)"sv },
	{ LR"(Defpoints)"sv,LR"(_wzdp)"sv,LR"(_qzdp)"sv },
	{ LR"(参考线)"sv,LR"(_wzr)"sv,LR"(_qzr)"sv },
	{ LR"(中心线)"sv,LR"(_wzc)"sv,LR"(_qzc)"sv },
	{ LR"(尺寸线)"sv,LR"(_wzs)"sv,LR"(_qzs)"sv },
	};
}

namespace {

	template<bool K, std::size_t N >
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

	template<bool K,std::size_t N>
	inline void Pack<K,N>::makeSeleted() {
		sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
		if (Acad::eOk == $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
			if (Acad::eOk != varBlockTable->getAt(ACDB_MODEL_SPACE,
				varBlockTableRecord,
				AcDb::kForRead)) {
				return;
			}
			_selete_a_block_table(varBlockTableRecord);
		}
	}

	template<bool K,std::size_t N>
	inline void Pack<K,N>::_selete_a_block_table(AcDbBlockTableRecord*varT) {
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
			const std::wstring_view varLayerName = pvarLayerName;
			if (varLayerName == globalNames[N].$LayerName) {
				$SeletedItems.insert(varEnt->objectId());
			}
			acutDelString(pvarLayerName);

		}
	}

	template<bool K,std::size_t N>
	inline void Pack<K,N>::hideObject() {
		if ($SeletedItems.empty()) { return; }
		class Lock {
		public:
			ads_name ss = {};
			Lock() { acedSSAdd(nullptr, nullptr, ss); }
			~Lock() { acedSSFree(ss); }
		} varLock;
		for (const auto & varI : $SeletedItems) {
			ads_name eName;
			acdbGetAdsName(eName, varI);
			acedSSAdd(eName, varLock.ss, varLock.ss);
		}
		if constexpr(K) {
			acedCommandS(RTSTR, L"_.hideobjects",
				RTPICKS, varLock.ss, RTSTR, L"", RTNONE);
		}
		else {
			acedCommandS(RTSTR, L"_.isolateobjects",
				RTPICKS, varLock.ss, RTSTR, L"", RTNONE);
		}
	}

	template<bool K,std::size_t N>
	Pack<K,N>::Pack() {
		$DB = acdbHostApplicationServices()->workingDatabase();
	}

	template<bool K, std::size_t A >
	class AddName {
	public:
		inline static void add_name() {
			Pack<K,A> pack;
			pack.makeSeleted();
			pack.hideObject();
		}
		inline static constexpr const wchar_t *globalName() { 
			if constexpr(K) {
				return globalNames[A].$FunctionName.data();
			}
			else {
				return globalNames[A].$FunctionName1.data();
			}
		}
		inline static constexpr const wchar_t *localName() { return globalName() + 1; }
	};

	template<typename T>
	inline void _p_arx_add_main_command() {
		acedRegCmds->addCommand(sstd::arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET,
			&T::add_name
		);
	}

}

#define _V_add_a_command(Nx) _p_arx_add_main_command<AddName<true,(Nx)>/**/>();_p_arx_add_main_command<AddName<false,(Nx)>/**/>()
namespace sstd {
	extern void loadHideLayer() {
		_V_add_a_command(0);
		_V_add_a_command(1);
		_V_add_a_command(2);
		_V_add_a_command(3);
		_V_add_a_command(4);
		_V_add_a_command(5);
		_V_add_a_command(6);
	}
}

/*************************************/





















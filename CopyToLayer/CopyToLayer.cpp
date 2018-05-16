/* 将图像原地复制到指定图层 */
#include "CopyToLayer.hpp"
#include <set>
#include <acedCmdNF.h>

void sstd::CopyToLayer::load() {
	arx_add_main_command_usepickset<CopyToLayer>();
}

static constexpr const wchar_t * layer_name() { return LR"(_坐标)"; }
namespace {
	class Concept {
	public:
		class Lock {
		public:
			ads_name ss = {};
			Lock() { acedSSAdd(nullptr, nullptr, ss); }
			~Lock() { acedSSFree(ss); }
		};
		Concept::Lock varLock;
	};
}/*namespace*/
static inline void select_data(Concept * arg) {
	const static class ThisLock {
	public:
		struct resbuf data[1];
		ThisLock() {
			data[0].rbnext = nullptr;
			data[0].restype = 8;
			data[0].resval.rstring = const_cast<wchar_t*>(
				LR"b...e(~_坐标)b...e");
		}
	} varFilter;

	acedSSGet(
		nullptr,
		nullptr,
		nullptr,
		varFilter.data,
		arg->varLock.ss);

	std::int32_t varLength = 0;
	acedSSLength(arg->varLock.ss, &varLength);
	if (varLength < 1) { svthrow(LR"(选择集为空)"); }

}

static inline void copy_to_layer(Concept * arg) {
	const constexpr static double varPoint[] = { 0.,0.,0.,0. };
	acedCommandS(
		RTSTR, L"COPYTOLAYER",
		RTPICKS, arg->varLock.ss,
		RTSTR, L"",
		RTSTR, L"_坐标",
		RT3DPOINT, varPoint,
		RT3DPOINT, varPoint,
		RTNONE);
}

void sstd::CopyToLayer::main() try {
	Concept concept;
	/*select object except the layer*/
	select_data(&concept);
	/*复制选中图像到指定图层*/
	copy_to_layer(&concept);

}
catch (...) {

}

namespace {
	template<bool LockLayer = true>
	static inline void lock_layers_except(const wchar_t * arg) {
		auto * DB = acdbHostApplicationServices()->workingDatabase();
		sstd::ArxClosePointer<AcDbLayerTable>varLayers;
		if (eOk != DB->getLayerTable(varLayers.pointer()))
			svthrow(LR"(can not open layer table)");

		class ResBuf : public resbuf {
		public:
			ResBuf(short value) {
				this->rbnext = nullptr;
				this->restype = RTSHORT;
				this->resval.rint = value;
			}
		};

		//CONSTRAINTINFER
		if constexpr(LockLayer) {
			const static ResBuf $constraintinfer/*1*/(0);
			acedSetVar(L"CONSTRAINTINFER",&$constraintinfer);
		}
		else {
			const static ResBuf $constraintinfer/*1*/(1);
			acedSetVar(L"CONSTRAINTINFER",&$constraintinfer);
		}

		if constexpr(LockLayer) {
			AcDbObjectId varLayerID;
			if (eOk != varLayers->getIdAt(arg, varLayerID)) {
				svthrow(LR"(do not have the layer)");
			}
			else {
				DB->setClayer(varLayerID);
			}
		}

		{
			std::unique_ptr<AcDbLayerTableIterator> varIt;
			{
				AcDbLayerTableIterator * tmp = nullptr;
				if (eOk != varLayers->newIterator(tmp))
					svthrow(LR"(can not get layer iterator)");
				varIt.reset(tmp);
			}
			for (; !varIt->done(); varIt->step()) {
				sstd::ArxClosePointer<AcDbLayerTableRecord> varLTR;
				if (eOk != varIt->getRecord(varLTR.pointer(), AcDb::kForWrite))
					continue;
				if constexpr(LockLayer) {
					AcString varLName;
					varLTR->getName(varLName);
					if (varLName != arg) {
						varLTR->setIsLocked(true);
					}
					else {
						varLTR->setIsLocked(false);
					}
				}
				else {
					(void)arg;
					varLTR->setIsLocked(false);
				}
			}
		}
		
	}

	class SCD {
	public:
		static void main()try {
			lock_layers_except<true>(layer_name());
			acDocManager->sendStringToExecute(acDocManager->curDocument(),
				LR"(Regen
)");
		}
		catch (...) {}
		DEFINE_ARX_NAME(LR"(_scd)")
	};

	class SCDD {
	public:
		static void main()try {
			lock_layers_except<false>(layer_name());
			acDocManager->sendStringToExecute(acDocManager->curDocument(),
				LR"(Regen
)");
		}
		catch (...) {}
		DEFINE_ARX_NAME(LR"(_scdd)")
	};


}/*打开关闭绘制模式*/

/****************************/
namespace sstd {
	extern void loadCopyToLayer() {
		CopyToLayer::load();
		arx_add_main_command<SCDD>();
		arx_add_main_command<SCD>();
	}
}



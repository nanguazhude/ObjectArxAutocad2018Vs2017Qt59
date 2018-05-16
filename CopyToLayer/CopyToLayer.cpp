/* 将图像原地复制到指定图层 */
#include "CopyToLayer.hpp"
#include <set>
#include <acedCmdNF.h>

void sstd::CopyToLayer::load() {
	arx_add_main_command_usepickset<CopyToLayer>();
}

static inline void refreshDisplay() {
	
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

		std::wstring varLockLayersNames;
		std::wstring varUnLockLayersNames;
		{
			auto * DB = acdbHostApplicationServices()->workingDatabase();
			sstd::ArxClosePointer<AcDbLayerTable>varLayers;
			if (eOk != DB->getLayerTable(varLayers.pointer(), kForWrite))
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
				acedSetVar(L"CONSTRAINTINFER", &$constraintinfer);
			}
			else {
				const static ResBuf $constraintinfer/*1*/(1);
				acedSetVar(L"CONSTRAINTINFER", &$constraintinfer);
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

					AcString varLName;
					varLTR->getName(varLName);

					if constexpr(LockLayer) {

						if (varLName != arg) {
							//varLTR->setIsLocked(true);
							varLockLayersNames.append(varLName, varLName.length());
							varLockLayersNames.push_back(L',');
						}
						else {
							//varLTR->setIsLocked(false);
							varUnLockLayersNames.append(varLName, varLName.length());
							varUnLockLayersNames.push_back(L',');
						}
					}
					else {
						(void)arg;
						//varLTR->setIsLocked(false);
						varUnLockLayersNames.append(varLName, varLName.length());
						varUnLockLayersNames.push_back(L',');
					}

					/*
					void refreshDisplay(){
						actrTransactionManager->queueForGraphicsFlush();
						actrTransactionManager->flushGraphics();
						acedUpdateDisplay();
					}
					*/
					/*  https://www.cnblogs.com/zhanglibo0626/archive/2012/12/28/2837136.html
					更新一个实体，用AcDbEntity::recordGraphicsModified() [[R方法可以特性表中修改属性后调用，close时刷新]] 和AcDbEntity::draw();
					对实体用AcDbEntity::recordGraphicsModified方法，使用Adesk::kTrue参数，实体关闭后，将UPDATE。
					void recordGraphicsModified(Adesk::Boolean setModified = Adesk::kTrue);
					setModified        Input Boolean value to indicate if the entity's graphics should be updated on screen when the object is closed
					This method sets the entity's state to indicate whether or not the entity should be
					updated on screen when it is closed. If the entity is set not to update,
					then any changes made will appear the next time the entity is regenerated.
					If setModified == Adesk::kFalse and the entity is currently set to update
					its graphics on close (which is its default state), then this function sets the entity
					so that it will not update on screen when it is closed and objectModified notification
					is sent to all entity type reactors attached to the entity.
					If setModified == Adesk::kFalse and the entity is already set not to update its
					graphics on close, then this function simply returns.
					If setModified == Adesk::kTrue, then the entity is set to update
					on screen when it is closed.
					*/
				}
			}
		}

		if ( varUnLockLayersNames.empty()==false ) {
			varUnLockLayersNames.pop_back();
			//varUnLockLayersNames.push_back(L'\n');

			acedCommandS(
				RTSTR, L"-LAYER",
				RTSTR, L"Unlock",
				RTSTR, varUnLockLayersNames.c_str(),
				RTSTR,LR"(
)",
				RTNONE);

		}

		if (varLockLayersNames.empty()==false) {
			varLockLayersNames.pop_back();
			//varLockLayersNames.push_back(L'\n');

			acedCommandS(
				RTSTR, L"-LAYER",
				RTSTR, L"Lock",
				RTSTR, varLockLayersNames.c_str(),
				RTSTR, LR"(
)",
				RTNONE);
		}

	}

	class SCD {
	public:
		static void main()try {
			lock_layers_except<true>(layer_name());
			refreshDisplay();
		}
		catch (...) {}
		DEFINE_ARX_NAME(LR"(_scd)")
	};

	class SCDD {
	public:
		static void main()try {
			lock_layers_except<false>(layer_name());
			refreshDisplay();
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



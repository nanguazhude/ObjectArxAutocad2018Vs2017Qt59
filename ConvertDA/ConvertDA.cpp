#include "ConvertDA.hpp"

template<bool value >
static inline void _sstd_ConvertDA_main() try {

	class ThisFunction {
	public:
		ads_name _ssname;
	public:
		ThisFunction() {
			ads_ssadd(nullptr, nullptr, _ssname);
		}
		~ThisFunction() {
			ads_ssfree(_ssname);
		}

	}varThisFunction;

	/* 选择注释对象 */
	acedSSGet(nullptr,
		nullptr,
		nullptr,
		nullptr,
		varThisFunction._ssname);
	/* 获得对象ID */
	std::list< sstd::ArxClosePointer< AcDbDimension > > $AllDimension;
	{
		std::int32_t varSSLength = 0;
		acedSSLength(varThisFunction._ssname, &varSSLength);
		if (varSSLength < 1) { return; }
		for (std::int32_t i = 0; i < varSSLength; ++i) {
			ads_name varEnt;
			acedSSName(varThisFunction._ssname, i, varEnt);
			AcDbObjectId varID;
			acdbGetObjectId(varID, varEnt);
			AcDbDimension *varDimension = nullptr;
			if (eOk == acdbOpenObject(varDimension, varID, AcDb::kForWrite)) {
				$AllDimension.emplace_back(varDimension);
			}
		}
	}
	/* 尝试进行转换 */
	{
		for (auto & varI : $AllDimension) {
			if (false == varI->isConstraintObject()) { continue; }
			varI->setConstraintDynamic(value);
		}
	}
}
catch (...) {
	return;
}

namespace {
	class ConvertDD {
	public:
		static void load() { sstd::arx_add_main_command_usepickset<ConvertDD>(); }
		static void main() { _sstd_ConvertDA_main<true>(); }
		DEFINE_ARX_NAME(LR"(_ccd)")
	};
}/**/

void sstd::ConvertDA::load() {
	arx_add_main_command_usepickset<ConvertDA>();
}

void sstd::ConvertDA::main() {
	_sstd_ConvertDA_main<false>();
}

namespace sstd {
	extern void loadConvertDA() {
		ConvertDA::load();
		ConvertDD::load();
	}
}/*sstd*/

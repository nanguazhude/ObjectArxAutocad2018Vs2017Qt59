#include "EPrint.hpp"
#include <acedCmdNF.h>
#include <string>
#include <memory>
#include <set>
#include <string_view>
using namespace std::string_view_literals;

namespace {
	class wstring :public std::wstring {
		using super = std::wstring;
	public:
		using super::super;
		wstring(const std::wstring_view & arg) :wstring(arg.data(), arg.size()) {}
	};
}/*namespace*/

namespace sstd {

	extern void loadEPrint() {
		EPrint::load();
	}

	void EPrint::load() {
		arx_add_main_command<EPrint>();
	}

	/*set block up*/
	template<bool ToUp=true>
	std::vector<AcDbObjectId > setBlockDrawOrder(
		AcDbDatabase*$DB,
		const wstring&varName) {

		sstd::ArxClosePointer< AcDbBlockTable> varBlockTable;
		if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) {
			svthrow(LR"(获得AcDbBlockTable失败1)"sv);
		}

		sstd::ArxClosePointer<AcDbBlockTableRecord> varR;
		if (eOk != varBlockTable->getAt(varName.c_str(),
			varR.pointer(),
			AcDb::kForRead,
			true)) {
			svthrow(LR"(获得AcDbBlockTable失败2)"sv);
		}

		std::unique_ptr<AcDbBlockReferenceIdIterator> varIt;
		{
			AcDbBlockReferenceIdIterator * varTmp;
			if (eOk != varR->newBlockReferenceIdIterator(varTmp)) {
				svthrow(LR"(获得迭代器失败)"sv);
			}
			varIt.reset(varTmp);
		}

		std::set< AcDbObjectId > varBID;
		for (; !varIt->done(); varIt->step()) {

			sstd::ArxClosePointer<AcDbBlockReference> varBR;
			if (eOk != varIt->getBlockReference(varBR, AcDb::kForWrite,true)) {
				continue;
			}

			varBID.insert(varBR->objectId());
			varBR->setLayer(LR"(尺寸线)");

		}

		if (varBID.empty()) { return{}; }

		{
			class Lock {
			public:
				ads_name ss = {};
				Lock() { construct(); }
				void construct() { acedSSAdd(nullptr, nullptr, ss); }
				void destory() { acedSSFree(ss); ss[0] = 0; ss[1] = 0; }
				~Lock() { destory(); }
			} varLock;

			for (const auto & varI:varBID) {
				ads_name s;
				acdbGetAdsName(s, varI);
				acedSSAdd(s, varLock.ss, varLock.ss);
			}

			if constexpr(ToUp) {
				acedCommandS(
					RTSTR, LR"(DRAWORDER)",
					RTPICKS, varLock.ss,
					RTSTR, L"",
					RTSTR, L"F",
					RTNONE);
			}
			else {
				acedCommandS(
					RTSTR, LR"(DRAWORDER)",
					RTPICKS, varLock.ss,
					RTSTR, L"",
					RTSTR, L"B",
					RTNONE);
			}

		}

		return{ varBID.begin(),varBID.end() };
	}

	void EPrint::main() try{
		auto DB = acdbHostApplicationServices()->workingDatabase();

		try {
			setBlockDrawOrder<false>(DB, LR"(@标题栏文字(1))"sv);
		}
		catch (...) {
			acutPrintf(LR"(e@标题栏文字(1)
)");
		}

		try {
			setBlockDrawOrder<true>(DB, LR"(横边框2(G3000))"sv);
		}
		catch (...) {
			acutPrintf(LR"(e@横边框2(G3000)
)");
		}
		
	}
	catch (...) {}

}/*namespace sstd*/





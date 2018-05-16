/* 将图像原地复制到指定图层 */
#include "CopyToLayer.hpp"
#include <set>
#include <acedCmdNF.h>

void sstd::CopyToLayer::load(){
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
		}  ;
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

	std::int32_t varLength=0;
	acedSSLength(arg->varLock.ss,&varLength);
	if (varLength < 1) { svthrow(LR"(选择集为空)"); }

}

static inline void copy_to_layer(Concept * arg) {
	const constexpr static double varPoint[] = {0.,0.,0.,0.};
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
	/**/
	copy_to_layer(&concept);
}
catch (...) {

}

/****************************/
namespace sstd{
	extern void loadCopyToLayer() { CopyToLayer::load(); }
}



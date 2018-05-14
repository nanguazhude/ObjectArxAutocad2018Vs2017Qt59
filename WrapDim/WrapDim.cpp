#include "WrapDim.hpp"

void sstd::WrapDim::load() {
	arx_add_main_command<WrapDim>();
}

namespace {

	class Resbuf : public  resbuf {
		/*
		Resbuf *rbnext; // Allows them to be "linked"
        short restype;
        union ads_u_val resval;
		*/
	public:
		Resbuf(Resbuf * a,short b,short c) {
			this->rbnext = a;
			this->restype = b;
			this->resval.rint = c;
		}
	};

	class TG {
	public:
		static void main() {
			auto varOsnap = get_osnap();
			if ((varOsnap->resval.rint) &&
				(/*打开osnap*/0 == (varOsnap->resval.rint & 0b0100'0000'0000'0000))) {
				*get_osnap_back() = *get_osnap();
				clear_osnap()/*关闭osnap*/;
			}
			else {
				static const Resbuf $osmode {
					nullptr, RTSHORT, static_cast<short>(0b0011'1111'1111'1101)
				};
				varOsnap = get_osnap_back();
				if (varOsnap->resval.rint &&
					(0/*打开osnap*/== (varOsnap->resval.rint & 0b0100'0000'0000'0000))) {
					set_osnap(varOsnap);
				}
				else {
					set_osnap(&$osmode);
				}
			}
		}

		constexpr static const wchar_t * name() {
			return LR"(OSMODE)";
		}

		static Resbuf * get_osnap_back() {
			static Resbuf $osmode {
				nullptr,
					RTSHORT,
					static_cast<std::int64_t>(0b0011'1111'1111'1101)
			};
			return &$osmode;
		}

		static const Resbuf * get_osnap() {
			static Resbuf $osmode {
				nullptr,
					RTSHORT,
					static_cast<short>(0)
			};
			acedGetVar(name(), &$osmode);
			return &$osmode;
		}

		static void set_osnap(const Resbuf *arg) {
			acedSetVar(name(), arg);
		}

		static void clear_osnap() {
			static const Resbuf $osmode {
				nullptr,
					RTSHORT,
					static_cast<std::int64_t>(0b0111'1111'1111'1101)
			};
			set_osnap(&$osmode);
		}

		static void load() {
			sstd::arx_add_main_command<TG>();
		}
		DEFINE_ARX_NAME(LR"(_tg)")
	};
}/**/

void sstd::WrapDim::main() {
	*TG::get_osnap_back() = *TG::get_osnap();
	TG::clear_osnap();
	/* 关闭自动捕捉 , 执行DIM */
	acDocManager->sendStringToExecute(acDocManager->curDocument(),
		LR"b...e(DIM
)b...e");
}

/*******************************/
namespace sstd {
	extern void loadWrapDim() {
		WrapDim::load();
		TG::load();
	}
}/*namespace sstd*/


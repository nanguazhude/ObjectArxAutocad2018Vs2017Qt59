#include "WrapDim.hpp"

void sstd::WrapDim::load() {
	arx_add_main_command<WrapDim>();
}

namespace {

	class TG {
	public:
		static void main() {
			auto varOsnap = get_osnap();
			if (varOsnap->resval.rint) {
				*get_osnap_back() = *get_osnap();
				clear_osnap();
			}
			else {
				static const struct resbuf $osmode {
					nullptr, RTSHORT, static_cast<std::int64_t>(16381)
				};
				if (get_osnap_back()->resval.rint) {
					set_osnap(get_osnap_back());
				}
				else {
					set_osnap(&$osmode);
				}
			}
		}

		constexpr static const wchar_t * name() {
			return LR"(OSMODE)";
		}

		static struct resbuf * get_osnap_back() {
			static struct resbuf $osmode;
			return &$osmode;
		}

		static const struct resbuf * get_osnap() {
			static struct resbuf $osmode {
				nullptr,
					RTSHORT,
					static_cast<short>(0)
			};
			acedGetVar(name(), &$osmode);
			return &$osmode;
		}

		static void set_osnap(const struct resbuf *arg) {
			acedSetVar(name(), arg);
		}

		static void clear_osnap() {
			static const struct resbuf $osmode {
				nullptr,
					RTSHORT,
					static_cast<std::int64_t>(0)
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


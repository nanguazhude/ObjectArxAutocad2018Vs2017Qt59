#include <tuple>
#include <vector>
#include "DimDistance.hpp"

namespace {

	template<std::size_t N/*N*/>
	class ResbufSet {
	public:
		struct resbuf $Data[N];
		operator const struct resbuf * () const { return static_cast<const struct resbuf *>($Data); }
		ResbufSet() = default;
		template<typename A0, typename ... Args>
		ResbufSet(A0&&a0, Args && ... args) {
			this->construct(std::forward<A0>(a0), std::forward<Args>(args)...);
		}
		template<typename ... Args>
		void construct(Args && ... args) {
			static_assert(N > 0, "N must bigger than zero");
			static_assert(sizeof ... (Args) / 2 == N, "N must equal sizeof(Args)/2");
			{
				this->__p_construct<0>(std::forward_as_tuple(std::forward<Args>(args)...));
			}
		}
	private:

		void __p_construct_detail(struct resbuf * d, short t, const wchar_t * const v) {
			d->restype = t;
			d->resval.rstring = const_cast<wchar_t *>(v);
		}

		void __p_construct_detail(struct resbuf * d, short t, ads_real v) {
			d->restype = t;
			d->resval.rreal = v;
		}

		void __p_construct_detail(struct resbuf * d, short t, short v) {
			d->restype = t;
			d->resval.rint = v;
		}

		void __p_construct_detail(struct resbuf * d, short t, int32_t v) {
			d->restype = t;
			d->resval.rlong = v;
		}

		void __p_construct_detail(struct resbuf * d, short t, int64_t v) {
			d->restype = t;
			d->resval.mnLongPtr = v;
		}

		void __p_construct_detail(struct resbuf * d, short t, const int64_t * const v) {
			d->restype = t;
			d->resval.rlname[0] = *v++;
			d->resval.rlname[1] = *v++;
		}

		void __p_construct_detail(struct resbuf * d, short t, const ads_real * const v) {
			d->restype = t;
			d->resval.rpoint[0] = *v++;
			d->resval.rpoint[1] = *v++;
			d->resval.rpoint[2] = *v++;
		}

		template<std::size_t n, typename Tuple>
		void __p_construct(const Tuple & t) {

			constexpr const static auto n0 = (n << 1);
			constexpr const static auto n1 = n0 + 1;

			this->__p_construct_detail(&(this->$Data[n]), std::get<n0>(t), std::get<n1>(t));

			constexpr const static auto varIsLast = (n == (N - 1));
			if constexpr(varIsLast) {
				this->$Data[n].rbnext = nullptr;
				return;
			}
			else {
				this->$Data[n].rbnext = &(this->$Data[n + 1]);
				return __p_construct<n + 1>(t);
			}
		}
	};

	class PrivateDimDistance {
	public:

		AcDbDatabase * $DB;
		class ObjectIndex {
		public:
			AcDbObjectId objectID;
			ads_name adsName;
			ObjectIndex() = default;
			ObjectIndex(const AcDbObjectId & a, ads_name b) {
				objectID = a;
				adsName[0] = *b;
				adsName[1] = *(1 + b);
			}
		};

		class Object : public ObjectIndex {
		};

		inline static std::vector<ObjectIndex> ssToAcDbObjects(ads_name ss) {
			std::int32_t varLen;
			acedSSLength(ss, &varLen);
			if (varLen < 1) { return{}; }

			std::vector<ObjectIndex> varAns;
			varAns.reserve(varLen);
			for (std::int32_t i = 0; i < varLen; ++i) {
				ads_name ent;
				acedSSName(ss, i, ent);
				AcDbObjectId eId;
				acdbGetObjectId(eId, ent);
				varAns.emplace_back(eId, ent);
			}

			return std::move(varAns);
		}

		Object $BasicObject;
		std::vector<Object> $Objects;

		PrivateDimDistance() {
			$DB = acdbHostApplicationServices()->workingDatabase();
		}

		void select_basic() {
			class Lock {
			public:
				ads_name ss = {};
				Lock() { acedSSAdd(nullptr, nullptr, ss); }
				~Lock() { acedSSFree(ss); }
			} varLock;

			const static ResbufSet<1> varSelectFilter{ RTDXF0,LR"(Dimension)" };
			acedSSGet(
				LR"(:S)"/*选择单个实例*/,
				nullptr,
				nullptr,
				varSelectFilter,
				varLock.ss
			);

			auto varObjecs = ssToAcDbObjects(varLock.ss);
			if (varObjecs.empty()) { throw 3423; }

			static_cast<ObjectIndex&>( $BasicObject ) = varObjecs[0];

		}

		void run() try {
			/*选择基*/
			select_basic();
			/*选择其它*/

		}
		catch (...) { return; }

	};
}/**/

namespace sstd {

	extern void loadDimDistance() {
		DimDistance::load();
	}

	void DimDistance::load() {
		arx_add_main_command_usepickset<DimDistance>();
	}

	void DimDistance::main() try {
		PrivateDimDistance var;
		var.run();
	}
	catch (...) {
		return;
	}

}/*namespace sstd*/

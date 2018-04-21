#include <tuple>
#include <vector>
#include <algorithm>
#include "DimDistance.hpp"

#include <string>
#include <string_view>

#include <acedCmdNF.h>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
	extern std::wstring_view double_to_string(double);
	extern std::wstring_view int_to_string(int);
}

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
		public:

			enum class DimType : int {
				AlignedDimension,
				RotatedDimension,
				None,
			};

			enum class Type :int {
				Limit/*极限公差*/,
				Normal/*无公差*/,
				Mirror/*对称公差*/,
				None,
			};

			DimType dimType = DimType::None;
			Type type = Type::None;
			AcGePoint3d varKeyPoint;

			void updateKeyPoint() {
				sstd::ArxClosePointer< AcDbEntity > varO;
				if (eOk != acdbOpenObject(varO, this->objectID)) {
					throw 332;
				}
				if (this->dimType == DimType::AlignedDimension) {
					auto var = AcDbAlignedDimension::cast(varO);
					if (var == nullptr) { throw nullptr; }
					this->varKeyPoint = var->dimLinePoint();
				}
				else if (this->dimType == DimType::RotatedDimension) {
					auto var = AcDbRotatedDimension::cast(varO);
					if (var == nullptr) { throw nullptr; }
					this->varKeyPoint = var->dimLinePoint();
				}
			}

			bool updateData() {
				sstd::ArxClosePointer< AcDbEntity > varO;
				if (eOk != acdbOpenObject(varO, this->objectID)) {
					throw 332;
				}

				if (varO->isKindOf(AcDbAlignedDimension::desc())) {
					dimType = DimType::AlignedDimension;
					auto var = AcDbAlignedDimension::cast(varO);
					if (var == nullptr) { return false; }
					{
						this->varKeyPoint = var->dimLinePoint();
					}
					{/* 根据公差样式来判断...type */
						auto dimSID = var->dimensionStyle();
						AcDbDimStyleTableRecord * varDimStyleObject = nullptr;
						if (eOk != acdbOpenObject(varDimStyleObject, dimSID)) {
							throw 7788;
						}

						if (varDimStyleObject->dimtol()) {
							if (varDimStyleObject->dimtp() == varDimStyleObject->dimtm()) {
								this->type = Type::Mirror;
							}
							else {
								this->type = Type::Limit;
							}
						}
						else {
							this->type = Type::Normal;
						}
						varDimStyleObject->close();
					}
					return true;
				}
				else if (varO->isKindOf(AcDbRotatedDimension::desc())) {
					dimType = DimType::RotatedDimension;
					auto var = AcDbRotatedDimension::cast(varO);
					if (var == nullptr) { return false; }
					{
						this->varKeyPoint = var->dimLinePoint();
					}
					{/* 根据公差样式来判断...type */
						auto dimSID = var->dimensionStyle();
						AcDbDimStyleTableRecord * varDimStyleObject = nullptr;
						if (eOk != acdbOpenObject(varDimStyleObject, dimSID)) {
							throw 7788;
						}

						if (varDimStyleObject->dimtol()) {
							if (varDimStyleObject->dimtp() == varDimStyleObject->dimtm()) {
								this->type = Type::Mirror;
							}
							else {
								this->type = Type::Limit;
							}
						}
						else {
							this->type = Type::Normal;
						}
						varDimStyleObject->close();
					}
					return true;
				}

				return false;
			}
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
		std::vector<Object *> $SortedObjects;

		PrivateDimDistance() {
			$DB = acdbHostApplicationServices()->workingDatabase();
		}

		void select_basic() {
			{
				class Lock {
				public:
					ads_name ss = {};
					Lock() { acedSSAdd(nullptr, nullptr, ss); }
					~Lock() { acedSSFree(ss); }
				} varLock;

				const static ResbufSet<1> varSelectFilter{ RTDXF0,LR"(Dimension)" };
				acutPrintf(LR"(选择基：
)");
				acedSSGet(
					LR"(:S)"/*选择单个实例*/,
					nullptr,
					nullptr,
					varSelectFilter,
					varLock.ss
				);

				auto varObjecs = ssToAcDbObjects(varLock.ss);
				if (varObjecs.empty()) { throw 3423; }

				static_cast<ObjectIndex&>($BasicObject) = varObjecs[0];
			}

			if (false == $BasicObject.updateData()) {
				throw 3321;
			}

		}

		void select_others() {

			std::vector<ObjectIndex> varObjecs;
			{
				$Objects.clear();
				class Lock {
				public:
					ads_name ss = {};
					Lock() { acedSSAdd(nullptr, nullptr, ss); }
					~Lock() { acedSSFree(ss); }
				} varLock;

				const static ResbufSet<1> varSelectFilter{ RTDXF0,LR"(Dimension)" };
				acutPrintf(LR"(选择其余：
)");
				acedSSGet(
					nullptr,
					nullptr,
					nullptr,
					varSelectFilter,
					varLock.ss
				);

				varObjecs = ssToAcDbObjects(varLock.ss);
				if (varObjecs.empty()) { throw 3423; }
			}

			$Objects.reserve(varObjecs.size());
			for (auto & varI : varObjecs) {
				auto & varJ = $Objects.emplace_back();
				static_cast<ObjectIndex&>(varJ) = varI;
				if (false == varJ.updateData()) {
					$Objects.pop_back();
					continue;
				}
			}
			if (varObjecs.empty()) { throw 3423; }
		}

		long double varDX = 0;
		long double varDY = 0;

		void sort_objects() {
			constexpr const static bool Ver = true;

			if constexpr(Ver) {

			}
			else {
				{/*sort them in autocad first*/
					class Lock {
					public:
						ads_name ss = {};
						Lock() { acedSSAdd(nullptr, nullptr, ss); }
						~Lock() { acedSSFree(ss); }
					} varLock;
					for (auto & varI : $Objects) {
						acedSSAdd(varI.adsName, varLock.ss, varLock.ss);
					}
					{
						/*_.DIMSPACE*/
						constexpr const static auto varCommand = LR"(_.DIMSPACE)";
						acedCommandS(
							RTSTR, varCommand,
							RTENAME, $BasicObject.adsName,
							RTPICKS, varLock.ss,
							RTSTR, L"",
							RTREAL, 3.141592654 * 5,
							RTNONE);
					}
				}

				{/*更新Key值*/
					for (auto & varI : $Objects) {
						varI.updateKeyPoint();
					}
				}
			}/*const expr*/

			$SortedObjects.clear();
			$SortedObjects.reserve(1 + this->$Objects.size());
			for (auto & varI : $Objects) {
				$SortedObjects.push_back(&varI);
			}
			$SortedObjects.push_back(&$BasicObject);


			if constexpr(Ver) {

			}
			else {
				varDX = 0;
				varDY = 0;
				/*计算X，Y方差*/
				{
					const double varISX = 1.0 / ($SortedObjects.size() < 333 ? ($SortedObjects.size()) : 333);
					const auto & varBK = $BasicObject.varKeyPoint;
					double varXMax = varBK.x; double varXMin = varBK.x;
					double varYMax = varBK.y; double varYMin = varBK.y;
					for (auto varI : $SortedObjects) {
						const auto & varK = varI->varKeyPoint;
						varDX += std::abs(varK.x - varBK.x)*varISX;
						varDY += std::abs(varK.y - varBK.y)*varISX;
						if (varK.x > varXMax) {
							varXMax = varK.x;
						}
						if (varK.x < varXMin) {
							varXMin = varK.x;
						}
						if (varK.y > varYMax) {
							varYMax = varK.y;
						}
						if (varK.y < varYMin) {
							varYMin = varK.y;
						}
					}
					if (varXMax != varXMin) varDX /= (varXMax - varXMin);
					if (varYMax != varYMin) varDY /= (varYMax - varYMin);
				}

				acutPrintf(LR"(
DX : %lf , DY : %lf
)", varDX, varDY);
			}/**/

#define sortBy std::sort/*std::stable_sort*/  
			if (varDX > varDY) {
				/*sort by x*/
				sortBy($SortedObjects.begin(), $SortedObjects.end(),
					[](Object * l, Object *r) {
					return (l->varKeyPoint.x < r->varKeyPoint.x);
				});
				if ($SortedObjects[0] == &$BasicObject) {
					return;
				}
				if (*$SortedObjects.rbegin() == &$BasicObject) {/*反序*/
					std::reverse($SortedObjects.begin(), $SortedObjects.end());
					return;
				}
				/*sort by y*/
				sortBy($SortedObjects.begin(), $SortedObjects.end(),
					[](Object * l, Object *r) {
					return (l->varKeyPoint.y < r->varKeyPoint.y);
				});
				if ($SortedObjects[0] == &$BasicObject) { return; }
				if (*$SortedObjects.rbegin() == &$BasicObject) {/*反序*/
					std::reverse($SortedObjects.begin(), $SortedObjects.end());
					return;
				}
				acutPrintf(LR"(基准尺寸不在最内或最外
)");
				throw 7834;
			}
			else {
				/*sort by y*/
				sortBy($SortedObjects.begin(), $SortedObjects.end(),
					[](Object * l, Object *r) {
					return (l->varKeyPoint.y < r->varKeyPoint.y);
				});
				if ($SortedObjects[0] == &$BasicObject) { return; }
				if (*$SortedObjects.rbegin() == &$BasicObject) {/*反序*/
					std::reverse($SortedObjects.begin(), $SortedObjects.end());
					return;
				}
				/*sort by x*/
				sortBy($SortedObjects.begin(), $SortedObjects.end(),
					[](Object * l, Object *r) {
					return (l->varKeyPoint.x < r->varKeyPoint.x);
				});
				if ($SortedObjects[0] == &$BasicObject) { return; }
				if (*$SortedObjects.rbegin() == &$BasicObject) {/*反序*/
					std::reverse($SortedObjects.begin(), $SortedObjects.end());
					return;
				}
				acutPrintf(LR"(基准尺寸不在最内或最外
)");
				throw 7834;
			}

		}

		static double space_0;// = 12/*用于非Limit*/;
		static double space_1;// = 16/*用于Limit*/;
		static void get_space() {

			auto getString0 = [varLastLength = space_0]()->std::wstring {
				std::wstring varAns = LR"(输入间距0<)"s;
				varAns += sstd::double_to_string(varLastLength);
				varAns += LR"(>:)"sv;
				return std::move(varAns);
			};

			auto getString1 = [varLastLength = space_1]()->std::wstring {
				std::wstring varAns = LR"(输入间距1<)"s;
				varAns += sstd::double_to_string(varLastLength);
				varAns += LR"(>:)"sv;
				return std::move(varAns);
			};

			{
				const std::wstring varString = getString0();
				const auto backData = space_0;
				auto varError = acedGetDist(nullptr, varString.c_str(), &space_0);
				if ((RTNORM == varError) && (space_0 > 0.)) {
					/*that is ok*/
				}
				else {
					if (varError == RTNONE) {
						space_0 = backData;
					}
					else {
						throw 3423;
					}
				}
			}

			{
				const std::wstring varString = getString1();
				const auto backData = space_1;
				auto varError = acedGetDist(nullptr, varString.c_str(), &space_1);
				if ((RTNORM == varError) && (space_1 > 0.)) {
					/*that is ok*/
				}
				else {
					if (varError == RTNONE) {
						space_1 = backData;
					}
					else {
						throw 3423;
					}
				}
			}

		}

		void apply() {
			auto varPos = $SortedObjects.cbegin();
			const auto varEPos = $SortedObjects.cend();
			auto varBasic = varPos ;

			Object::Type varThisType = Object::Type::None;
			while (varPos != varEPos) {

				varBasic = varPos++;

				class Lock {
				public:
					ads_name ss = {};
					Lock() { acedSSAdd(nullptr, nullptr, ss); }
					~Lock() { acedSSFree(ss); }
				} varLock;

				{
					auto varP = (*varBasic);
					varThisType = varP->type;
				}

				for (; varPos != varEPos; ++varPos) {
					auto varP = (*varPos);
					if (varP->type == varThisType) {
						acedSSAdd(varP->adsName, varLock.ss, varLock.ss);
						continue;
					}
					else {
						acedSSAdd(varP->adsName, varLock.ss, varLock.ss);
						break;
					}
				}

				{
					/*_.DIMSPACE*/
					constexpr const static auto varCommand = LR"(_.DIMSPACE)";
					acedCommandS(
						RTSTR, varCommand,
						RTENAME, (*varBasic)->adsName,
						RTPICKS, varLock.ss,
						RTSTR, L"",
						RTREAL, (varThisType == Object::Type::Limit) ? space_1 : space_0,
						RTNONE);
				}					

			}/*while*/

		}/*apply*/

		void run(long double a, long double b) try {
			varDX = a;
			varDY = b;
			/*选择基*/
			select_basic();
			/*选择其它*/
			select_others();
			/*sort*/
			sort_objects();
			/*获得对齐值*/
			//get_space();
			/*apply*/
			apply();
		}
		catch (...) { return; }

	};

	double PrivateDimDistance::space_0 = 12;
	double PrivateDimDistance::space_1 = 16;

	class DimDistanceB {
	public:
		static void main()try {
			PrivateDimDistance var;
			var.run(0, 1);
		}
		catch (...) {

		}
		DEFINE_ARX_NAME(LR"(_DDBDIMDISTANCE)")

	};/**/

	class DimDistanceC {
	public:
		static void main()try {
			PrivateDimDistance::get_space();
		}
		catch (...) {

		}
		DEFINE_ARX_NAME(LR"(_DDDDIMDISTANCE)")

	};/**/

}/**/

namespace sstd {

	extern void loadDimDistance() {
		DimDistance::load();
	}

	void DimDistance::main() try {
		PrivateDimDistance var;
		var.run(1, 0);
	}
	catch (...) {
		return;
	}

	void DimDistance::load() {
		arx_add_main_command_usepickset<DimDistance>();
		arx_add_main_command_usepickset<DimDistanceB>();
		arx_add_main_command_usepickset<DimDistanceC>();
	}

}/*namespace sstd*/

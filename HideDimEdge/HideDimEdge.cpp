#include <list>
#include <optional>
#include <object_arx_global.hpp>
#include "HideDimEdge.hpp"

namespace sstd {

	extern void loadHideDimEdge() { HideDimEdge::load(); }

	namespace {

		class SSName {
			using __T = std::optional< ads_namep >;
			__T $data;
		public:
			inline SSName() {}
			inline SSName(ads_namep arg) : $data(arg) {}
			inline ~SSName() { if ($data) { acedSSFree(*$data); } }
			operator ads_namep() {
				if ($data) { return *$data; }
				return nullptr;
			}
			operator const ads_namep() const {
				if ($data) { return *$data; }
				return nullptr;
			}
			operator bool()const { return bool($data); }
			SSName & operator=(ads_namep arg) {
				$data = arg;
				return *this;
			}
			SSName(const SSName &) = delete;
			SSName(SSName &&) = delete;
			SSName&operator=(const SSName &) = delete;
			SSName&operator=(SSName &&) = delete;
		};

		class ThisFunction final {
		public:
			ads_name varSSName;
			AcDbDatabase * const $DB;
			SSName $Select;
			ThisFunction(AcDbDatabase * arg) :$DB(arg) {}
			inline bool make_select();
			std::list< sstd::ArxClosePointer< AcDbDimension > > $AllDimension;
			inline void ThisFunction::make_dim_set();
			template<bool, bool >
			inline void update();
		};

		inline void ThisFunction::make_dim_set() {
			std::int32_t varSSLength = 0;
			acedSSLength(ads_namep($Select), &varSSLength);
			for (std::int32_t i = 0; i < varSSLength; ++i) {
				ads_name varEnt;
				acedSSName(ads_namep($Select), i, varEnt);
				AcDbObjectId varID;
				acdbGetObjectId(varID, varEnt);
				AcDbDimension *varDimension = nullptr;
				if (eOk == acdbOpenObject(varDimension, varID, AcDb::kForWrite)) {
					$AllDimension.emplace_back(varDimension);
				}
			}
		}

		inline bool  ThisFunction::make_select() {
			/*ACRX_CMD_USEPICKSET|ACRX_CMD_REDRAW*/
			acedSSGet(nullptr,
				nullptr,
				nullptr,
				nullptr,
				varSSName);
			std::int32_t varSSLength = 0;
			acedSSLength(varSSName, &varSSLength);
			if (varSSLength < 1) { return false; }
			$Select = varSSName;
			return true;
		}

		template<bool h1, bool h2>
		inline void ThisFunction::update() {
			for (auto & varI : $AllDimension) {
				varI->setDimse1(h1);
				varI->setDimse2(h2);
			}
		}

		template<bool h1, bool h2>
		inline void _p_update() {
			ThisFunction varThis(acdbHostApplicationServices()->workingDatabase());
			if (false == varThis.make_select()) {
				return;
			}
			varThis.make_dim_set();
			varThis.update<h1, h2>();
		}

		class HideDimEdge1 {
		public:
			static void main() { _p_update<true, false>(); }
			DEFINE_ARX_NAME(LR"(_hidedimedge1)")
		};
		class HideDimEdge2 {
		public:
			static void main() { _p_update<false, true>(); }
			DEFINE_ARX_NAME(LR"(_hidedimedge2)")
		};

		class HideDimEdge0 {
		public:
			static void main() { _p_update<false, false>(); }
			DEFINE_ARX_NAME(LR"(_hidedimedge0)")
		};

	}/*namespace*/

}/*namespace sstd*/

sstd::HideDimEdge::HideDimEdge() {

}

void sstd::HideDimEdge::load() {
	arx_add_main_command_usepickset<HideDimEdge>();
	arx_add_main_command_usepickset<HideDimEdge1>();
	arx_add_main_command_usepickset<HideDimEdge2>();
	arx_add_main_command_usepickset<HideDimEdge0>();
}

void sstd::HideDimEdge::main() {
	_p_update<true, true>();

}



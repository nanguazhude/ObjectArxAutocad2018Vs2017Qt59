#include <map>
#include <dbmleaderstyle.h>
#include <object_arx_global.hpp>
#include "UpdateMLeaderStyle.hpp"

namespace sstd {

	UpdateMLeaderStyle::UpdateMLeaderStyle() {

	}

	namespace {
		class _UpdateMLeaderStyle {
		public:
			AcDbDatabase * const $DB;
			sstd::ArxClosePointer<AcDbDictionary> $MleaderTable;
			using Function = void(*)(_UpdateMLeaderStyle* ,const std::wstring & , AcDbMLeaderStyle * );
			using Functions = std::map<std::wstring, std::pair<Function, bool>>;
			Functions $Functions;
			_UpdateMLeaderStyle(AcDbDatabase*arg) :$DB(arg){}
			inline void update();
		private:
			void _construct();
		};

		inline void _UpdateMLeaderStyle::update() {
			_construct();

			if (kOk != $DB->getMLeaderStyleDictionary(
				$MleaderTable.pointer(), AcDb::kForWrite)) {
				return;
			}

			std::unique_ptr<AcDbDictionaryIterator> 
				varIt{ $MleaderTable->newIterator() };

			if (varIt == nullptr) { return; }

			const auto varNoPos = $Functions.end();
			for (;!varIt->done();varIt->next()) {
				sstd::ArxClosePointer<AcDbMLeaderStyle> varR;

				if(kOk!= varIt->getObject(varR.pointer(), AcDb::kForWrite)){
					continue;
				}

				std::wstring varNameW;
				{
					wchar_t * varName = nullptr;
					if (kOk != varR->getName(varName)) {
						return;
					}
					varNameW = varName;
				}

				auto varPos = $Functions.find(varNameW);

				if (varPos == varNoPos) { continue; }
				varPos->second.second = true;

				varPos->second.first(this, varNameW, varR);

			}

			for (auto & varIt: $Functions) {
				if (varIt.second.second) { continue; }
				varIt.second.second = true;
				varIt.second.first(this, varIt.first, nullptr);
			}

		}

		void _UpdateMLeaderStyle::_construct() {
			$Functions.clear();
			$Functions.emplace(LR"()"s, Functions::value_type::second_type{ [](_UpdateMLeaderStyle*argTable,
				const std::wstring &argName,
				AcDbMLeaderStyle *argR) {
				sstd::ArxClosePointer<AcDbMLeaderStyle> varLocal;
				if (argR==nullptr) {
					AcDbObjectId varID;
					argR = new AcDbMLeaderStyle;
					varLocal = argR;
					argR->setName(argName.data());
					argTable->$MleaderTable->setAt(argName.data(),
						argR, varID);
				}
				/****************************************************************/
				/****************************************************************/
			} ,false});
		}

	}/*namespace*/

	extern void loadUpdateMLeaderStyle() { 
		arx_add_main_command<UpdateMLeaderStyle>(); }

	void UpdateMLeaderStyle::load(){
	}

	void UpdateMLeaderStyle::main(){
		_UpdateMLeaderStyle varThisFunction( 
			acdbHostApplicationServices()->workingDatabase());
		varThisFunction.update();
	}

}/*namespace sstd*/

#include <map>
#include <optional>
#include <dbmleaderstyle.h>
#include <object_arx_global.hpp>
#include "UpdateMLeaderStyle.hpp"

namespace sstd {

	UpdateMLeaderStyle::UpdateMLeaderStyle() {
	}

	namespace {
		class _UpdateMLeaderStyle {
		public:
			std::optional< AcDbObjectId > $TextTypeID;
			AcDbDatabase * const $DB;
			sstd::ArxClosePointer<AcDbDictionary> $MleaderTable;
			using Function = void(*)(_UpdateMLeaderStyle*, const std::wstring &, AcDbMLeaderStyle *);
			using Functions = std::map<std::wstring, std::pair<Function, bool>>;
			Functions $Functions;
			_UpdateMLeaderStyle(AcDbDatabase*arg) :$DB(arg) {}
			inline void update();
		private:
			inline void _textTypeID();
			inline void _construct();
		};

		inline void _UpdateMLeaderStyle::_textTypeID() {
			AcDbTextStyleTable * varT;
			if (kOk == $DB->getTextStyleTable(varT)) {
				AcDbObjectId varID;
				if (kOk == varT->getAt(LR"(@Standard)", varID)) {
					$TextTypeID = varID;
				}
				varT->close();
			}
		}

		inline void _UpdateMLeaderStyle::update() {
			_construct();

			if (kOk != $DB->getMLeaderStyleDictionary(
				$MleaderTable.pointer(), AcDb::kForWrite)) {
				acutPrintf(LR"(getMLeaderStyleDictionary
)");
				return;
			}
			else {
				acutPrintf(LR"(getMLeaderStyleDictionary : ok
)");
			}

			std::unique_ptr<AcDbDictionaryIterator>
				varIt{ $MleaderTable->newIterator() };

			if (varIt == nullptr) { return; }

			const auto varNoPos = $Functions.end();
			for (; !varIt->done(); varIt->next()) {
				sstd::ArxClosePointer<AcDbMLeaderStyle> varR;

				if (kOk != varIt->getObject(varR.pointer(), AcDb::kForWrite)) {
					continue;
				}

				std::wstring varNameW;
				{
					wchar_t * varName = nullptr;
					if (kOk != varR->getName(varName)) {
						continue;
					}
					varNameW = varName;
				}

				auto varPos = $Functions.find(varNameW);

				if (varPos == varNoPos) { continue; }
				varPos->second.second = true;

				varPos->second.first(this, varNameW, varR);

			}

			for (auto & varIt : $Functions) {
				if (varIt.second.second) { continue; }
				varIt.second.second = true;
				varIt.second.first(this, varIt.first, nullptr);
			}

		}

		inline void _UpdateMLeaderStyle::_construct() {
			if (false == bool($TextTypeID))_textTypeID();
			$Functions.clear();
			$Functions.emplace(LR"(Test)"s,
				Functions::value_type::second_type{ [](_UpdateMLeaderStyle*argTable,
				const std::wstring &argName,
				AcDbMLeaderStyle *argR) {
				sstd::ArxClosePointer<AcDbMLeaderStyle> varLocal;
				if (argR == nullptr) {
					AcDbObjectId varID;
					argR = new AcDbMLeaderStyle;
					varLocal = argR;
					argR->setName(argName.data());
					argTable->$MleaderTable->setAt(argName.data(),
						argR, varID);
				}
				/****************************************************************/
				argR->setAnnotative(true)/*注释性*/;
				argR->setArrowSize(4.6)/*箭头大小*/;
				argR->setMaxLeaderSegmentsPoints(2);
				argR->setBreakSize(0)/**/;
				argR->setDescription(LR"(Hellow Kitty!)");
				argR->setContentType(AcDbMLeaderStyle::kMTextContent);
				if (argTable->$TextTypeID) argR->setTextStyleId(*argTable->$TextTypeID);
				argR->setTextAngleType(AcDbMLeaderStyle::kHorizontalAngle);
				argR->setTextAttachmentDirection(AcDbMLeaderStyle::kAttachmentHorizontal);
				argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kLeftLeader);
				argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kRightLeader);
				argR->setExtendLeaderToText(true);
				argR->setLandingGap(1.2);
				argR->setEnableDogleg(false);
				argR->setArrowSymbolId(LR"()");
				/****************************************************************/
			} ,false });
		}

	}/*namespace*/

	extern void loadUpdateMLeaderStyle() {
		UpdateMLeaderStyle::load();
	}

	void UpdateMLeaderStyle::load() {
		arx_add_main_command<UpdateMLeaderStyle>();
	}

	void UpdateMLeaderStyle::main() {
		_UpdateMLeaderStyle varThisFunction(
			acdbHostApplicationServices()->workingDatabase());
		varThisFunction.update();
	}

}/*namespace sstd*/

/**************************************************************/

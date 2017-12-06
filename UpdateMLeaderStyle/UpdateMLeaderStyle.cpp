#include <map>
#include <optional>
#include <dbmleaderstyle.h>
#include <object_arx_global.hpp>
#include "UpdateMLeaderStyle.hpp"

namespace sstd {

	UpdateMLeaderStyle::UpdateMLeaderStyle() {
	}

	namespace {

		inline AcCmColor operator""_ac(unsigned long long arg) {
			AcCmColor varAns;
			varAns.setColorIndex(static_cast<std::uint16_t>(arg));
			return std::move(varAns);
		}

		class _UpdateMLeaderStyle {
		public:
			std::optional< AcDbObjectId > $TextTypeID;
			std::optional< AcDbObjectId > $ArrowID;
			AcDbDatabase * const $DB;
			AcDbDictionary * $MleaderTable = nullptr;
			using Function = void(*)(_UpdateMLeaderStyle*, const std::wstring &, AcDbMLeaderStyle *);
			using Functions = std::map<std::wstring, std::pair<Function, bool>>;
			Functions $Functions;
			_UpdateMLeaderStyle(AcDbDatabase*arg) :$DB(arg) {}
			inline void update();
			~_UpdateMLeaderStyle() {
				if ($MleaderTable) { $MleaderTable->close(); }
			}
		private:
			inline void _arrowTypeID();
			inline void _textTypeID();
			inline void _construct();
		};

		inline void _UpdateMLeaderStyle::_arrowTypeID() {
			AcDbBlockTable * varT;
			if (kOk == $DB->getBlockTable(varT)) {
				AcDbObjectId varID;
				if (kOk == varT->getAt(LR"(_MY_ROW)", varID)) {
					$ArrowID = varID;
				}
				varT->close();
			}
		}

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
				$MleaderTable, AcDb::kForWrite)) {
				acutPrintf(LR"(getMLeaderStyleDictionary
)");
				return;
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

				std::wstring varNameW{ varIt->name() };
				auto varPos = $Functions.find(varNameW);
				if (varPos == varNoPos) { continue; }

				acutPrintf(LR"(已有引线样式：)");
				acutPrintf(varNameW.c_str());
				acutPrintf(LR"(
)");

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
			if (false == bool($ArrowID)) _arrowTypeID();
			$Functions.clear();
#include "appendMLeaderStyle/Annotative.hpp"
#include "appendMLeaderStyle/Standard.hpp"
#include "appendMLeaderStyle/引线Point.hpp"
#include "appendMLeaderStyle/无箭头引线.hpp"
			/*///////////////////////////////////////////////////////////////////////////////*/
			$Functions.emplace(LR"(引线标注)"s,
				Functions::value_type::second_type{ [](_UpdateMLeaderStyle*argTable,
				const std::wstring &argName,
				AcDbMLeaderStyle *argR) {
				sstd::ArxClosePointer<AcDbMLeaderStyle> varLocal;
				//AcDbMLeaderStyle * varLocal = nullptr;
				if (argR == nullptr) {
					argR = new AcDbMLeaderStyle;
					varLocal = argR;
					argR->setName(argName.c_str());
				}
				/****************************************************************/
				argR->setTextHeight(6.75);
				argR->setAnnotative(true)/*注释性*/;
				argR->setArrowSize(4.6)/*箭头大小*/;
				argR->setMaxLeaderSegmentsPoints(2);
				argR->setBreakSize(0)/**/;
				//argR->setDescription(LR"(Hellow Kitty!)");
				argR->setContentType(AcDbMLeaderStyle::kMTextContent);
				if (argTable->$TextTypeID) argR->setTextStyleId(*argTable->$TextTypeID);
				argR->setTextAngleType(AcDbMLeaderStyle::kHorizontalAngle);
				argR->setTextAttachmentDirection(AcDbMLeaderStyle::kAttachmentHorizontal);
				argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kLeftLeader);
				argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kRightLeader);
				argR->setExtendLeaderToText(true);
				argR->setLandingGap(1.2);
				argR->setEnableDogleg(false);
				if (argTable->$ArrowID) {
					argR->setArrowSymbolId(*(argTable->$ArrowID))/*箭头样式*/;
				}
				else {
					argR->setArrowSymbolId(LR"()")/*箭头样式*/;
				}
				argR->setTextColor(40_ac);
				argR->setLeaderLineColor(11_ac);
				AcDbObjectId varID;
				//argR->postMLeaderStyleToDb(argTable->$DB, argName.c_str(),varID);
				if (varLocal) {
					argTable->$MleaderTable->setAt(argName.c_str(),
						argR, varID);
				}
				/****************************************************************/
			} ,false });
			/*///////////////////////////////////////////////////////////////////////////////*/

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

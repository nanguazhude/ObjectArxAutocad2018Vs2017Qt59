/**
选择直线 ,
找到与直线相交的对象 [圆,直线] ,
计算直线与这些对象的交点集合 ,
在交点集合中找到距离最大的一组交点 ,
用直线连接交点
**/
#include "DrawCrossLine.hpp"
#include <set>
#include <vector>

void sstd::DrawCrossLine::load() {
	arx_add_main_command<DrawCrossLine>();
}

namespace {
	class DrawCrossLinePrivate {
		const AcDbObjectId $LineObject;
		const AcGePoint3d $LineStart;
		const AcGePoint3d $LineEnd;
		std::set<AcDbObjectId> $Items;
		class PointCompare {
		public:
			bool operator()(const AcGePoint3d & l, const AcGePoint3d & r) const {
				if (l.x < r.x) { return true; }
				if (l.x > r.x) { return false; }
				return l.y < r.y;
			}
		};
		std::set<AcGePoint3d, PointCompare> $Points;
	public:
		DrawCrossLinePrivate(
			const AcDbObjectId  &argObjectID,
			const AcGePoint3d & argLineStart,
			const AcGePoint3d &argLineEnd) :
			$LineObject(argObjectID),
			$LineStart(argLineStart),
			$LineEnd(argLineEnd) {}
		void exec() {

			const bool varDX = $LineStart.x > $LineEnd.x;
			const bool varDY = $LineStart.y > $LineEnd.y;
			const bool varDXE = ($LineStart.x == $LineEnd.x);
			const bool varDYE = ($LineStart.y == $LineEnd.y);

			if (varDXE&&varDYE) {
				svthrow(LR"(input two pionts are equal)");
			}

			class Lock {
			public:
				ads_name varE = {};
				Lock() { acedSSAdd(nullptr, nullptr, varE); }
				~Lock() { acedSSFree(varE); }
			}varSelectSet;

			{
				struct resbuf varPointlist[2];
				varPointlist[0].restype = RTPOINT;
				varPointlist[0].rbnext = &varPointlist[1];

				constexpr const static auto varDiff = 0.0001;

				varPointlist[0].resval.rpoint[0] = $LineStart.x + (varDXE ? 0 : (varDX ? varDiff : -varDiff));
				varPointlist[0].resval.rpoint[1] = $LineStart.y + (varDYE ? 0 : (varDY ? varDiff : -varDiff));
				varPointlist[0].resval.rpoint[2] = 0;

				varPointlist[1].restype = RTPOINT;
				varPointlist[1].rbnext = nullptr;
				varPointlist[1].resval.rpoint[0] = $LineEnd.x - (varDXE ? 0 : (varDX ? varDiff : -varDiff));
				varPointlist[1].resval.rpoint[1] = $LineEnd.y - (varDYE ? 0 : (varDY ? varDiff : -varDiff));
				varPointlist[1].resval.rpoint[2] = 0;

				acedSSGet(LR"...(F)..."/*Fence (open polygon) selection */,
					varPointlist/*point list*/,
					nullptr/*null*/,
					nullptr/*filter*/,
					varSelectSet.varE);
			}

			{
				std::int32_t varLen;
				acedSSLength(varSelectSet.varE, &varLen);
				if (varLen < 2) { svthrow(LR"...(the select set is null)..."); }
				for (std::int32_t i = 0; i < varLen; ++i) {
					ads_name ent;
					acedSSName(varSelectSet.varE, i, ent);
					AcDbObjectId eId;
					acdbGetObjectId(eId, ent);
					$Items.insert(eId);
				}
				$Items.erase($LineObject);
				if ($Items.size() < 1) {
					svthrow(LR"...(the select set is null)...");
				}
			}

			sstd::ArxClosePointer<  AcDbEntity  > varLine;
			if (eOk != acdbOpenObject(varLine.pointer(), $LineObject, AcDb::kForRead)) {
				svthrow(LR"...(can not open line object)...");
			}

			for (const auto & varI : $Items) {
				sstd::ArxClosePointer<  AcDbEntity  > varItem;
				if (eOk != acdbOpenObject(varItem.pointer(), varI, AcDb::kForRead)) {
					continue;
				}
				AcGePoint3dArray varIPoints;
				varItem->intersectWith(varLine, kOnBothOperands, varIPoints);
				if (varIPoints.isEmpty()) { continue; }
				for (const auto & varII : varIPoints)$Points.insert(varII);
			}
			/**/
			AcGePoint3d varPoint0, varPoint1;
			if (varDXE) {/*按照y排序*/
				const auto varPointsPair = std::minmax_element($Points.begin(), $Points.end(),
					[](const auto & l, const auto &r) {return l.y < r.y; });
				varPoint0 = *varPointsPair.first;
				varPoint1 = *varPointsPair.second;
			}
			else {/*按照x排序*/
				const auto varPointsPair = std::minmax_element($Points.begin(), $Points.end(),
					[](const auto & l, const auto &r) {return l.x < r.x; });
				varPoint0 = *varPointsPair.first;
				varPoint1 = *varPointsPair.second;
			}

			/*add the line to db*/
			sstd::ArxClosePointer<AcDbLine> varAnsLine = new AcDbLine(varPoint0, varPoint1);
			varAnsLine->setLayer(LR"b...e(粗实线)b...e");

			{
				auto varDB = acdbHostApplicationServices()
					->workingDatabase();

				{
					sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
					sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

					{
						auto varE = varDB->getBlockTable(varBlockTable, AcDb::kForRead);
						if (varE != eOk) { svthrow(LR"(打开BlockTable失败)"sv); }
						varE = varBlockTable->getAt(ACDB_MODEL_SPACE,
							varBlockTableRecord,
							AcDb::kForWrite);
						if (varE != eOk) { svthrow(LR"(打开模型空间失败)"sv); }
					}

					varBlockTableRecord->appendAcDbEntity(varAnsLine);

				}

			}

		}
	};

	void do_draw_cross_line(
		const AcDbObjectId  &argObjectID,
		const AcGePoint3d & argLineStart,
		const AcGePoint3d & argLineEnd)try {
		auto && varWorker =
			std::make_unique<DrawCrossLinePrivate>(
				argObjectID,
				argLineStart,
				argLineEnd);
		varWorker->exec();
	}
	catch (...) {}

}/*namespace*/

void sstd::DrawCrossLine::main() {
	/*we get the line */

}

namespace sstd {
	extern void loadDrawCrossLine() {
		DrawCrossLine::load();
	}
}/*namespace sstd*/

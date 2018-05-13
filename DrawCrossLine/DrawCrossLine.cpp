/**
选择直线 ,
找到与直线相交的对象 [圆,直线...] ,
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

		AcGePoint3d $PPP0;
		AcGePoint3d $PPP1;
		AcGePoint3d $PPP2;
		AcGePoint3d $PPP3;

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

		void get_4_points() {
			constexpr const static auto varHW = 0.00085/*对于一般机械行业,这个精度是合适的*/;
			auto varDx = $LineStart.x - $LineEnd.x;
			auto varDy = $LineStart.y - $LineEnd.y;
			auto varLength = varHW / std::hypot(varDx, varDy);
			varDx *= varLength;
			varDy *= varLength;

			$PPP0 = $LineStart;
			$PPP0.x += varDx - varDy;
			$PPP0.y += varDy + varDx;

			$PPP1 = $LineEnd;
			$PPP1.x += -varDx - varDy;
			$PPP1.y += -varDy + varDx;

			$PPP3 = $LineStart;
			$PPP3.x += varDx + varDy;
			$PPP3.y += varDy - varDx;

			$PPP2 = $LineEnd;
			$PPP2.x += -varDx + varDy;
			$PPP2.y += -varDy - varDx;

			if constexpr(false)/*debug*/{
				auto varDB = acdbHostApplicationServices()
					->workingDatabase();

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

				sstd::ArxClosePointer<AcDbLine> varAnsLine1 = new AcDbLine($PPP0, $PPP1);
				sstd::ArxClosePointer<AcDbLine> varAnsLine2 = new AcDbLine($PPP1, $PPP2);
				sstd::ArxClosePointer<AcDbLine> varAnsLine3 = new AcDbLine($PPP2, $PPP3);
				sstd::ArxClosePointer<AcDbLine> varAnsLine4 = new AcDbLine($PPP3, $PPP0);
				
				varAnsLine1->setLayer(LR"(参考线)");
				varAnsLine2->setLayer(LR"(参考线)");
				varAnsLine3->setLayer(LR"(参考线)");
				varAnsLine4->setLayer(LR"(参考线)");
				
				varBlockTableRecord->appendAcDbEntity(varAnsLine1);
				varBlockTableRecord->appendAcDbEntity(varAnsLine2);
				varBlockTableRecord->appendAcDbEntity(varAnsLine3);
				varBlockTableRecord->appendAcDbEntity(varAnsLine4);

			}/*debug*/

		}

		void exec() {

			//const bool varDX = $LineStart.x > $LineEnd.x;
			//const bool varDY = $LineStart.y > $LineEnd.y;
			const bool varDXE = ($LineStart.x == $LineEnd.x);
			const bool varDYE = ($LineStart.y == $LineEnd.y);

			if (varDXE&&varDYE) {
				svthrow(LR"(input two pionts are equal)");
			}
			get_4_points();

			class Lock {
			public:
				ads_name varE = {};
				Lock() { acedSSAdd(nullptr, nullptr, varE); }
				~Lock() { acedSSFree(varE); }
			}varSelectSet;

			{
				struct resbuf varPointlist[4];
				varPointlist[0].restype = RTPOINT;
				varPointlist[0].rbnext = &varPointlist[1];
				varPointlist[0].resval.rpoint[0] = $PPP0.x;
				varPointlist[0].resval.rpoint[1] = $PPP0.y;
				varPointlist[0].resval.rpoint[2] = 0;

				varPointlist[1].restype = RTPOINT;
				varPointlist[1].rbnext = &varPointlist[2];
				varPointlist[1].resval.rpoint[0] = $PPP1.x;
				varPointlist[1].resval.rpoint[1] = $PPP1.y;
				varPointlist[1].resval.rpoint[2] = 0;

				varPointlist[2].restype = RTPOINT;
				varPointlist[2].rbnext = &varPointlist[3];
				varPointlist[2].resval.rpoint[0] = $PPP2.x;
				varPointlist[2].resval.rpoint[1] = $PPP2.y;
				varPointlist[2].resval.rpoint[2] = 0;

				varPointlist[3].restype = RTPOINT;
				varPointlist[3].rbnext = nullptr;
				varPointlist[3].resval.rpoint[0] = $PPP3.x;
				varPointlist[3].resval.rpoint[1] = $PPP3.y;
				varPointlist[3].resval.rpoint[2] = 0;

				acedSSGet(LR"...(CP)..."/*Fence (open polygon) selection */,
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
				sstd::ArxClosePointer< AcDbEntity > varItem;
				if (eOk != acdbOpenObject(varItem.pointer(), varI, AcDb::kForRead)) {
					continue;
				}
				if (varItem->isKindOf(AcDbPoint::desc()))continue/*忽略点*/;
				if (varItem->isKindOf(AcDbDimension::desc()))continue/*忽略标注*/;
				if (varItem->isKindOf(AcDbMText::desc()))continue/*忽略多行文字*/;
				if (varItem->isKindOf(AcDbText::desc()))continue/*忽略单行文字*/;
				if (varItem->isKindOf(AcDbMline::desc()))continue/*忽略引线*/;
				if (varItem->isKindOf(AcDbHatch::desc()))continue/*忽略填充*/;
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

void sstd::DrawCrossLine::main() try {

	{
		class Lock {
		public:
			ads_name varE = {};
			Lock() { acedSSAdd(nullptr, nullptr, varE); }
			~Lock() { acedSSFree(varE); }
		}varSelectSet;
		double varPointsTmp[4];
		sstd::ArxClosePointer< AcDbLine > L;
		/*we get the line */
		if (RTNORM == acedEntSel(LR"(选择一条直线
)", varSelectSet.varE, varPointsTmp)) {
			AcDbObjectId eId;
			acdbGetObjectId(eId, varSelectSet.varE);   //获取实体id  
			AcDbEntity * pEnt = nullptr;
			if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForWrite)) {
				svthrow(LR"(打开实体失败)"sv);//打开实体失败，返回  
			}
			if (pEnt && pEnt->isKindOf(AcDbLine::desc())) {
				L = AcDbLine::cast(pEnt);
				/*call draw line*/
				const auto varID = L->objectId();
				const auto varSP = L->startPoint();
				const auto varEP = L->endPoint();
				L.close()/*close the line now*/;
				do_draw_cross_line(varID, varSP, varEP);
			}
			else {
				if (pEnt) pEnt->close();
				svthrow(LR"(不是直线)"sv);
			}
		}
		else { svthrow(LR"(选择直线失败)"sv); }
	}
}
catch (...) {}

namespace sstd {
	extern void loadDrawCrossLine() {
		DrawCrossLine::load();
	}
}/*namespace sstd*/

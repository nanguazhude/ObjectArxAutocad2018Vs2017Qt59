#include "EChamfer.hpp"
namespace sstd{
EChamfer::EChamfer(){
}

void EChamfer::load(){
	arx_add_main_command<EChamfer>();
}

void EChamfer::main(){

}

/****************/
namespace {
	namespace __get_three_points {

		void getALine(double *P, AcDbLine*&L) {
			class Lock {
			public:
				ads_name varE;
				~Lock() { acedSSFree(varE); }
			}ss;
			
			if (RTNORM == acedEntSel(LR"(选择一条直线)", ss.varE, P)) {
				AcDbObjectId eId;
				acdbGetObjectId(eId, ss.varE );   //获取实体id  
				AcDbEntity * pEnt;
				if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForRead)) {
					throw 222;//打开实体失败，返回  
				}
				if (pEnt->isKindOf(AcDbLine::desc())) {
					L = AcDbLine::cast(pEnt);
				}
				else {
					throw 333;
				}
			}
			else { throw 111; }
		}

		bool get_three_point(
			AcGePoint3d&argStart,
			AcGePoint3d&argCenter,
			AcGePoint3d&argEnd
		) try{
			AcGePoint3d varPointFistLine;
			AcGePoint3d varPointSecondLine;
			AcGePoint3d varPointIntersect;
			sstd::ArxClosePointer<AcDbLine> varFirstLine;
			sstd::ArxClosePointer<AcDbLine> varSecondLine;
			{
				/*获得直线1******************/
				getALine(&varPointFistLine.x, varFirstLine);
			}
			{
				/*获得直线2******************/
				getALine(&varPointSecondLine.x, varSecondLine);
			}
			{
				/*计算两条直线的交点*********/
				AcGePoint3dArray varAns;
				varSecondLine->intersectWith(varFirstLine.pointer(),
					AcDb::Intersect::kExtendBoth,
					varAns);
				if (varAns.isEmpty()) { throw 555; }
				varPointIntersect = varAns.first();
			}

			argCenter = varPointIntersect;

			return true;
		}
		catch (...) { return false; }
	}
}/*namespace*/
/****************/

}/*namespace sstd*/




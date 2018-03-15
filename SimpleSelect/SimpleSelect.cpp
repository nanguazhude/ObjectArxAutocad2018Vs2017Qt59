#include "SimpleSelect.hpp"

namespace sstd {
	extern void loadSimpleSelect() {
		SimpleSelect::load();
	}
}

namespace sstd {

	SimpleSelect::SimpleSelect() {
	}

	void SimpleSelect::load() {
		arx_add_main_command_usepickset<SimpleSelect>();
	}

	namespace {
		void _p_select_a_line() {
			bool varLoop = true;
			AcGePoint3d $LineStartPoint;
			AcGePoint3d $LineEndPoint;

			/*获得直线*/
			while (varLoop) {
				class Lock {
				public:
					ads_name ssName = {};
					~Lock() {
						acedSSFree(ssName);
					}
				}lock;
				double varPoint[4];
				struct resbuf *insStack;
				ads_matrix adsmat;
				AcDbObjectId varID;//acedNEntSelP
				if (RTNORM == acedNEntSelP(LR"("选择直线:
)", lock.ssName, varPoint, false, adsmat, &insStack)) {
					acdbGetObjectId(varID, lock.ssName);   //获取实体id  
					sstd::ArxClosePointer< AcDbEntity > pEnt;
					if (Acad::eOk != acdbOpenObject(pEnt, varID, AcDb::kForRead)) {
						acutPrintf(LR"(打开实体失败
)");
						return;//打开实体失败，返回  
					}

					if (pEnt->isKindOf(AcDbLine::desc())) {
						AcDbLine * pLine = AcDbLine::cast(pEnt);
						$LineStartPoint = pLine->startPoint();
						$LineEndPoint = pLine->endPoint();
						/*************************************/
						/*QString test;
						test += ":x:";
						test += QString::number($SPoint.x);
						test += ":y:";
						test += QString::number($SPoint.y);
						test += "\n";

						test += ":x:";
						test += QString::number($LineStartPoint.x);
						test += ":y:";
						test += QString::number($LineStartPoint.y);
						test += "\n";

						test += ":x:";
						test += QString::number($LineEndPoint.x);
						test += ":y:";
						test += QString::number($LineEndPoint.y);
						test += "\n";

						auto j = test.toStdWString();
						acutPrintf(j.c_str());*/
						/*************************************/
					}
					else {
						acutPrintf(LR"(请选择直线
)");
					}
				}
				else {
					acutPrintf(LR"(取消选择
)");
					return;
				}
			}

		}
	}

	void SimpleSelect::main() {
		_p_select_a_line();
		//int varFlag = -1;
		/*  */
//		acutPrintf(LR"(
//1 : 选择一条直线测试
//)");

		/*if (RTNORM == acedGetInt(LR"()", &varFlag)) {
			switch (varFlag)
			{
			case 1:_p_select_a_line(); return; 
			default:break;
			}
		}*/

		/*acutPrintf(LR"(输入错误
)");*/
	}

}/*sstd*/






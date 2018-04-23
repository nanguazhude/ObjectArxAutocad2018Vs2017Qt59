#include "FARC.hpp"
#include "FARC_ALG.hpp"
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"
#include <string_view>

using namespace std::string_view_literals;

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
	extern std::wstring_view double_to_string(double);
	extern std::wstring_view int_to_string(int);
}

namespace {
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {
	extern void loadFARC() {
		FARC::load();
	}
}/*namespace sstd*/

namespace sstd {

	FARC::FARC() {

	}

	void FARC::load() {
		arx_add_main_command<FARC>();
	}

	void FARC::main() try {
		AcDbObjectId varAID;
		AcGePoint3d $EndPoint;
		AcGePoint3d $StartPoint;
		AcGePoint3d $CenterPoint;
		double $R = 1.6;
		{
			/*获得数据库*/
			auto varDB = acdbHostApplicationServices()
				->workingDatabase();
			class LockMain {
			public:
				AcDbDatabase * db;
				AcDbObjectId lid;
				LockMain(AcDbDatabase *a) :db(a) {
					lid = a->clayer();
					{
						AcDbObjectId llid;
						sstd::ArxClosePointer<AcDbLayerTable>t;
						a->getLayerTable(t);
						if (bool(t) == false) { throw 557000; }
						t->getAt(LR"(粗实线)", llid);
						if (llid.isNull() == false)a->setClayer(llid);
					}
				}
				~LockMain() {
					db->setClayer(lid);
				}
			}____{ varDB };

			AcGePoint3d $LineStartPoint;
			AcGePoint3d $LineEndPoint;
			AcGePoint3d $X0Point;
			AcGePoint3d $SPoint;

#if 0
			bool varLoop = true;
			/*获得直线*/
			while (varLoop) {
				class Lock {
				public:
					ads_name ssName = {};
					Lock() { acedSSAdd(nullptr, nullptr, ssName); }
					~Lock() {
						acedSSFree(ssName);
					}
				}lock;
				double varPoint[4];
				AcDbObjectId varID;//acedNEntSelP
				if (RTNORM == acedEntSel(LR"("选择直线:
)", lock.ssName, varPoint)) {
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
						$SPoint.x = varPoint[0];
						$SPoint.y = varPoint[1];
						$SPoint.z = 0;
						$SPoint = UcsToWorld($SPoint);
						varLoop = false;

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
#endif

			/*获得起点*/
			{
				const auto varError =
					acedGetPoint(nullptr, LR"(请输入切线起点<0,0>：)", &($LineStartPoint.x));

				if (RTNONE == varError) {
					$LineStartPoint = { 0.0,0.0,0.0 };
				}
				else if (varError != RTNORM) {
					throw varError;
				}

				$LineStartPoint = UcsToWorld($LineStartPoint);
			}

			/*获得直线上另一点*/
			{
				const auto varError =
					acedGetPoint(nullptr, LR"(请输入切线终点<0,0>：)", &($LineEndPoint.x));

				if (RTNONE == varError) {
					$LineEndPoint = { 0.0,0.0,0.0 };
				}
				else if (varError != RTNORM) {
					throw varError;
				}

				$LineEndPoint = UcsToWorld($LineEndPoint);
			}

			$SPoint = $LineStartPoint;

			/*获得点*/
			{
				const auto varError =
					acedGetPoint(nullptr, LR"(请输入起始中心<0,0>：)", &($X0Point.x));

				if (RTNONE == varError) {
					$X0Point = { 0.0,0.0,0.0 };
				}
				else if (varError != RTNORM) {
					throw varError;
				}

				$X0Point = UcsToWorld($X0Point);
			}

			/*指定半径*/
			do {
				static double last_R = 1.6;
				std::wstring varTmpString = LR"(请输入半径<)"s;
				varTmpString += sstd::double_to_string(last_R);
				varTmpString += LR"(>:)"sv;
				//acutPrintf(varTmpString.c_str());
				int varReturn = acedGetReal(varTmpString.c_str(), &$R);
				if (RTNONE == varReturn) {
					$R = last_R;
					break;
				}
				else if ((RTNORM == varReturn) &&
					($R > std::numeric_limits<double>::epsilon())) {
					last_R = $R;
					break;
				}

				acutPrintf(LR"(您输入了一个无效值
)");
				return;
			} while (false);

			/*计算*/
			{
				double varTmpData[8];
				if (false == sstd::_private_FARC_ALG_HPP::eval<double>(
					$LineStartPoint.x, $LineStartPoint.y,
					$LineEndPoint.x, $LineEndPoint.y,
					$X0Point.x, $X0Point.y,
					$SPoint.x, $SPoint.y,
					$R,
					varTmpData)) {
					acutPrintf(LR"(计算失败
)");
					return;
				}
				else {
					$CenterPoint.x = varTmpData[0];
					$CenterPoint.y = varTmpData[1];

					$StartPoint.x = varTmpData[2];
					$StartPoint.y = varTmpData[3];

					$EndPoint.x = varTmpData[4];
					$EndPoint.y = varTmpData[5];
				}
			}

			/*绘图*/
			{
				sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
				sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

				{
					auto varE = varDB->getBlockTable(varBlockTable, AcDb::kForRead);
					if (varE != eOk) { throw varE; }
					varE = varBlockTable->getAt(ACDB_MODEL_SPACE,
						varBlockTableRecord,
						AcDb::kForWrite);
					if (varE != eOk) { throw varE; }
				}

				sstd::ArxClosePointer< AcDbArc > varArc{
					new AcDbArc{ $CenterPoint,
					$R,
					std::atan2($StartPoint.y - $CenterPoint.y,$StartPoint.x - $CenterPoint.x),
					std::atan2($EndPoint.y - $CenterPoint.y,$EndPoint.x - $CenterPoint.x)
				} };

				varBlockTableRecord->appendAcDbEntity(varAID, varArc);

			}
		}

		/*添加约束*/
		{
			AcDbObjectId varTmp;
			AcGePoint3d varMiddlePoint = $EndPoint;
			varMiddlePoint.x += $StartPoint.x;
			varMiddlePoint.y += $StartPoint.y;
			{
				auto dx = 0.5*varMiddlePoint.x - $CenterPoint.x;
				auto dy = 0.5*varMiddlePoint.y - $CenterPoint.y;
				const auto l = std::hypot(dx, dy);
				dx /= l;
				dy /= l;
				varMiddlePoint.x = std::fma(dx, $R, $CenterPoint.x);
				varMiddlePoint.y = std::fma(dy, $R, $CenterPoint.y);
			}
			AcDbAssoc2dConstraintAPI::createRadialDimConstraint(
				varAID,
				varMiddlePoint,
				{ 0.5*($CenterPoint.x + varMiddlePoint.x),0.5*(varMiddlePoint.y + $CenterPoint.y),0 },
				varTmp);
		}

	}
	catch (...) {
		acutPrintf(LR"(FARC : thres is a undo exception
)");
	}

}/*namespace sstd*/




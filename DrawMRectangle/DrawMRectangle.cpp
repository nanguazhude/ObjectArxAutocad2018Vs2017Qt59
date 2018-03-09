#include <object_arx_global.hpp>
#include <vector>
#include <array>
#include <optional>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"
#include "DrawMRectangle.hpp"
#include <QtQml/QtQml>

namespace sstd {

	DrawMRectangle::DrawMRectangle() {

	}

	void DrawMRectangle::load() {
		arx_add_main_command<DrawMRectangle>();
	}

	namespace {
		namespace _p000_ {
			static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
				AcGeMatrix3d ucs;
				acedGetCurrentUCS(ucs);
				AcGePoint3d res(ucsPoint);
				return res.transformBy(ucs);
			}
						
			class Rect {
			public:
				double $Width;
				double $Height;
				Rect(double a, double b) :$Width(a), $Height(b) {}
				Rect() :$Width(0), $Height(0) {}
			};

			class PrivatePack {
			public:
				QString $FileName;
				AcGePoint3d $StartPoint;
				AcDbDatabase * $DB = nullptr;
				int $Error = 0;
				std::vector<Rect> $Rects;
			};

			inline void check_error(PrivatePack * arg, int varG) {
				if (arg->$Error == varG) { return; }
				throw arg->$Error;
			}

			inline bool _p_construct_pack(PrivatePack * arg) {
				arg->$DB = acdbHostApplicationServices()->workingDatabase();
				return true;
			}

			inline bool _p_select_point(PrivatePack * arg) try {
				arg->$Error = acedGetPoint(nullptr, LR"(请输入起始中心<0,0>：)", &(arg->$StartPoint.x));
				if (RTNONE == arg->$Error) { arg->$StartPoint = { 0.0,0.0,0.0 }; }
				else { check_error(arg, RTNORM); }
				return true;
			}
			catch (...) {
				return false;
			}

			inline bool _p_select_file(PrivatePack * arg) try {
				
				QString varFileNameQ;
				{
					const wchar_t * varFileName
						= acDocManager
						->mdiActiveDocument()
						->fileName();
					if (varFileName == nullptr) { throw 6; }
					varFileNameQ = QString::fromWCharArray(varFileName);
				}
				arg->$FileName = QFileDialog::getOpenFileName(nullptr, {},
					QFileInfo(varFileNameQ).absolutePath());
				if (arg->$FileName.isEmpty()) {
					throw 0;
				}
				return true;
			}
			catch (...) {
				return false;
			}

			/*
			w
			123
			45
			h
			78
			32
			*/
			inline bool _p_update_data(PrivatePack * arg) try {
				
				std::list<double> w;
				std::list<double> h;
				{
					QJSEngine varJSEngine;
					QFile varFile{ arg->$FileName };
					if (false == varFile.open(QFile::ReadOnly)) {
						throw 1;
					}
					QTextStream varStream{ &varFile };
					//bool varHRead = false;
					//bool varWRead = false;
					int varIsH = -10;
					/* 0:H 1:W 2:J */

					while (varStream.atEnd() == false) {
						const QString varLine = varStream
							.readLine()
							.trimmed()
							.toLower();

						/*空值跳过*/
						if (varLine.isEmpty()) { continue; }

						if (varLine.startsWith( QStringLiteral(R"(:h)") ) ) {
							varIsH = 0 ; 
							//varHRead = true; 
							continue;
						}
						else if ( varLine.startsWith( QStringLiteral(R"(:w)") ) ) {
							varIsH = 1 ; 
							//varWRead = true; 
							continue;
						}
						else if (varLine.startsWith( QStringLiteral(R"(:j)") ) ) {
							varIsH = 2;
							continue;
						}

						if (2==varIsH) {
							varJSEngine.evaluate(varLine);
							continue;
						}

						const auto varValue =
							varJSEngine.evaluate(varLine).toNumber();

						if (0 == varIsH) {
							h.push_back(varValue);
						}
						else if(1 == varIsH ){ 
							w.push_back(varValue);
						}
					}
				}
				if (w.size() != h.size()) { throw 2; }
				if (w.empty()) { throw 3; }
				arg->$Rects.clear();
				arg->$Rects.reserve(w.size());
				auto varBw = w.begin();
				auto varE = w.end();
				auto varBh = h.begin();
				for (; varE != varBw; ++varBw, ++varBh) {
					arg->$Rects.emplace_back(*varBw, *varBh);
				}
				return true;
			}
			catch (...) {
				return false;
			}

			class DrawARectPack {
			public:
				AcGePoint3d varKeyPoint0;
				AcGePoint3d varKeyPoint1;
				AcGePoint3d varKeyPoint2;
				AcGePoint3d varKeyPoint3;
				AcGePoint3d varKeyPoint4;
				AcGePoint3d varKeyPoint5;

				AcGePoint3d varKeyPoint0_1;
				AcGePoint3d varKeyPoint1_2;
				AcGePoint3d varKeyPoint2_3;
				AcGePoint3d varKeyPoint3_4;
				AcGePoint3d varKeyPoint4_5;
				AcGePoint3d varKeyPoint5_0;

				using LineType = sstd::ArxClosePointer< AcDbLine >;
				std::array<LineType, 6> varLines;
				sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
				sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;

				AcDbObjectId varID0;
				AcDbObjectId varID1;
				AcDbObjectId varID2;
				AcDbObjectId varID3;
				AcDbObjectId varID4;
				AcDbObjectId varID5;

				std::vector<AcDbObjectId> varCObjecs;
				AcCmColor varColor;
			};

			inline auto add_scale_half(const double & a, const double &b) {
				return (a + b)*0.5;
			}

			inline auto mid(const AcGePoint3d & x, const AcGePoint3d &y) {
				return AcGePoint3d(add_scale_half(x.x, y.x),
					add_scale_half(x.y, y.y),
					0.
				);
			}

			inline void _p_draw_a_rect(const Rect & argR,
				DrawARectPack & argP,
				PrivatePack & arg) {

				argP.varColor.setRGB(154 + std::rand() % (245 - 154),
					50 + std::rand() % (80 - 50),
					200 + std::rand() % (250 - 200));

					{
						arg.$Error = arg.$DB->getBlockTable(argP.varBlockTable, AcDb::kForRead);
						check_error(&arg, eOk);
						arg.$Error = argP.varBlockTable->getAt(ACDB_MODEL_SPACE, argP.varBlockTableRecord,
							AcDb::kForWrite);
						check_error(&arg, eOk);
					}										

					const auto & varW = argR.$Width;
					const double varHH = argR.$Height*0.5;

					argP.varKeyPoint0 = arg.$StartPoint;
					argP.varKeyPoint1 = arg.$StartPoint;
					argP.varKeyPoint1.y += varHH;
					argP.varKeyPoint2 = argP.varKeyPoint1;
					argP.varKeyPoint2.x -= varW;
					argP.varKeyPoint3 = arg.$StartPoint;
					argP.varKeyPoint3.x = argP.varKeyPoint2.x;
					argP.varKeyPoint4 = argP.varKeyPoint3;
					argP.varKeyPoint4.y -= varHH;
					argP.varKeyPoint5 = argP.varKeyPoint4;
					argP.varKeyPoint5.x = argP.varKeyPoint0.x;

					argP.varLines[0] = { new AcDbLine{ argP.varKeyPoint0 ,argP.varKeyPoint1 } };
					argP.varLines[1] = { new AcDbLine{ argP.varKeyPoint1 ,argP.varKeyPoint2 } };
					argP.varLines[2] = { new AcDbLine{ argP.varKeyPoint2 ,argP.varKeyPoint3 } };
					argP.varLines[3] = { new AcDbLine{ argP.varKeyPoint3 ,argP.varKeyPoint4 } };
					argP.varLines[4] = { new AcDbLine{ argP.varKeyPoint4 ,argP.varKeyPoint5 } };
					argP.varLines[5] = { new AcDbLine{ argP.varKeyPoint5 ,argP.varKeyPoint0 } };

					argP.varKeyPoint0_1 = mid(argP.varKeyPoint0, argP.varKeyPoint1);
					argP.varKeyPoint1_2 = mid(argP.varKeyPoint1, argP.varKeyPoint2);
					argP.varKeyPoint2_3 = mid(argP.varKeyPoint2, argP.varKeyPoint3);
					argP.varKeyPoint3_4 = mid(argP.varKeyPoint3, argP.varKeyPoint4);
					argP.varKeyPoint4_5 = mid(argP.varKeyPoint4, argP.varKeyPoint5);
					argP.varKeyPoint5_0 = mid(argP.varKeyPoint5, argP.varKeyPoint0);

					/*添加线到数据库*/
					{
						int i = 0;
						for (auto & varI : argP.varLines) {
							switch (i) {
							case 0: {
								argP.varBlockTableRecord->appendAcDbEntity(argP.varID0, varI);
								argP.varCObjecs.push_back(argP.varID0);
							} break;
							case 1:argP.varBlockTableRecord->appendAcDbEntity(argP.varID1, varI); break;
							case 2:argP.varBlockTableRecord->appendAcDbEntity(argP.varID2, varI); break;
							case 3:argP.varBlockTableRecord->appendAcDbEntity(argP.varID3, varI); break;
							case 4:argP.varBlockTableRecord->appendAcDbEntity(argP.varID4, varI); break;
							case 5:argP.varBlockTableRecord->appendAcDbEntity(argP.varID5, varI); break;
							}
							++i;
						}
					}

					/*close */
					for (auto & varI : argP.varLines) {
						varI->setLayer(LR"(Defpoints)");
						varI->setColor(argP.varColor);
						varI = {};
					}
					argP.varBlockTableRecord = {};
					argP.varBlockTable = {};

			}

			inline void _p_constraint(DrawARectPack & argP) {
				/*添加约束*/
				argP.varKeyPoint0 = UcsToWorld(argP.varKeyPoint0);
				argP.varKeyPoint1 = UcsToWorld(argP.varKeyPoint1);
				argP.varKeyPoint2 = UcsToWorld(argP.varKeyPoint2);
				argP.varKeyPoint3 = UcsToWorld(argP.varKeyPoint3);
				argP.varKeyPoint4 = UcsToWorld(argP.varKeyPoint4);
				argP.varKeyPoint5 = UcsToWorld(argP.varKeyPoint5);
				argP.varKeyPoint0_1 = UcsToWorld(argP.varKeyPoint0_1);
				argP.varKeyPoint1_2 = UcsToWorld(argP.varKeyPoint1_2);
				argP.varKeyPoint2_3 = UcsToWorld(argP.varKeyPoint2_3);
				argP.varKeyPoint3_4 = UcsToWorld(argP.varKeyPoint3_4);
				argP.varKeyPoint4_5 = UcsToWorld(argP.varKeyPoint4_5);
				argP.varKeyPoint5_0 = UcsToWorld(argP.varKeyPoint5_0);
				/*添加重合约束*/
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID0, argP.varID1, argP.varKeyPoint1, argP.varKeyPoint1);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID1, argP.varID2, argP.varKeyPoint2, argP.varKeyPoint2);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID2, argP.varID3, argP.varKeyPoint3, argP.varKeyPoint3);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID3, argP.varID4, argP.varKeyPoint4, argP.varKeyPoint4);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID4, argP.varID5, argP.varKeyPoint5, argP.varKeyPoint5);
				AcDbAssoc2dConstraintAPI::createCoincidentConstraint(argP.varID5, argP.varID0, argP.varKeyPoint0, argP.varKeyPoint0);

				/*添加水平，垂直约束*/
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID1, argP.varKeyPoint1_2);
				AcDbAssoc2dConstraintAPI::createHorizontalConstraint(argP.varID4, argP.varKeyPoint4_5);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID0, argP.varKeyPoint0_1);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID2, argP.varKeyPoint2_3);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID3, argP.varKeyPoint3_4);
				AcDbAssoc2dConstraintAPI::createVerticalConstraint(argP.varID5, argP.varKeyPoint5_0);

				/*添加相同约束*/
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID0, argP.varID5, argP.varKeyPoint0_1, argP.varKeyPoint5_0);
				AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(argP.varID2, argP.varID3, argP.varKeyPoint2_3, argP.varKeyPoint3_4);

				/*添加尺寸约束*/
				AcDbObjectId varTmp;
				const auto varCPosR = mid(argP.varKeyPoint1, argP.varKeyPoint4);
				AcGePoint3d varCPos = varCPosR;
				AcDbAssoc2dConstraintAPI::createHorizontalDimConstraint(argP.varID0, argP.varID2,
					argP.varKeyPoint0_1, argP.varKeyPoint2_3, varCPos, varTmp);
				varCPos = varCPosR;
				AcDbAssoc2dConstraintAPI::createVerticalDimConstraint(argP.varID1, argP.varID4,
					argP.varKeyPoint1_2, argP.varKeyPoint4_5, varCPos, varTmp);
			}

			inline void _p_draw(PrivatePack * arg) {
				std::unique_ptr<DrawARectPack> varDAP{ new DrawARectPack };
				varDAP->varCObjecs.reserve(arg->$Rects.size());
				const auto varPosS = UcsToWorld(arg->$StartPoint);

				for (const auto &varI : arg->$Rects) try {
					_p_draw_a_rect(varI, *varDAP, *arg);
					_p_constraint(*varDAP);
				}
				catch (...) {
					return;
				}

				auto varPos0 = varDAP->varCObjecs.begin();
				auto varPos1 = varPos0 + 1;
				auto varEnd = varDAP->varCObjecs.end();

				for (; varPos1 != varEnd; varPos0 = varPos1++) {
					AcDbAssoc2dConstraintAPI::createCoincidentConstraint(*varPos0,
						*varPos1,
						varPosS,
						varPosS);
				}

			}

		}/*namespace _p000_*/
	}/*namespace*/


	void DrawMRectangle::main() {
		std::unique_ptr< QtApplication > varQApp{ new QtApplication };

		using namespace _p000_;
		PrivatePack varData;
		/*construc pack*/
		if (false == _p_construct_pack(&varData)) {
			return;
		}
		/*select point*/
		if (false == _p_select_point(&varData)) {
			return;
		}
		/*select file*/
		if (false == _p_select_file(&varData)) {
			return;
		}
		/*update data*/
		if (false == _p_update_data(&varData)) {
			return;
		}

		class Lock {
			PrivatePack * d;
			AcDbObjectId layerID;
		public:
			Lock(PrivatePack *v) :d(v) {
				layerID = d->$DB->clayer();
				d->$DB->setClayer(d->$DB->layerZero());
			}
			~Lock() {
				d->$DB->setClayer(layerID);
			}
		};
		Lock __lock{ &varData };

		/*draw*/
		_p_draw(&varData);
	}

	extern void loadDrawMRectangle() {
		DrawMRectangle::load();
	}

}/*namespace sstd*/

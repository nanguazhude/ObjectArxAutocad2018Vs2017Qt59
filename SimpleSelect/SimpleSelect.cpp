#include "SimpleSelect.hpp"


extern int acedNEntSelPEx(const ACHAR *str,
	ads_name entres,
	ads_point ptres, int pickflag,
	ads_matrix xformres,
	struct resbuf **refstkres,
	unsigned int uTransSpaceFlag,
	intptr_t* gsmarker);

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

		static inline void pressEnterToContinue(){
			thread_local TCHAR tempBuf[256];
			acedGetString(0, L"\nPress <ENTER> to continue.", tempBuf);
		}
		
		class SelectALine {
		public:
			static inline int extractEntityInfo(
				struct resbuf *rb,
				int&          sel_method,
				ads_name      ename,
				ads_name      subname,
				short&        marker,
				AcGePoint3d&  pickpnt,
				AcGeVector3d& pickvec) {
				constexpr const auto  CONTINUE = 1;
				constexpr const auto  CANCEL = 0;
				constexpr const auto  PICK_METH = 1;
				constexpr const auto  FENCE_METH = 4;

				if (rb == nullptr || rb->restype != RTLB) {
					return 0;
				}

				// Get the selection method.
				//
				rb = rb->rbnext;  // Bump up to the selection method, always after RTLB.
				sel_method = rb->resval.rint;

				// Get the first ename (could be either the actual entity name or
				// subentity name).
				//
				rb = rb->rbnext;  // Bump up to the first name, always after sel method.
				ename[0] = rb->resval.rlname[0];
				ename[1] = rb->resval.rlname[1];
				subname[0] = rb->resval.rlname[0];
				subname[1] = rb->resval.rlname[1];

				// Get marker info.
				//
				rb = rb->rbnext;
				marker = rb->resval.rint;

				// Get the pick location and vector, only for PICK and FENCE.  For FENCE,
				// we take the first intersection with the entity as the pick location.
				//
				if (sel_method == PICK_METH ||
					sel_method == FENCE_METH) {
					rb = rb->rbnext;  // Skip to RTLB for pick location.
					rb = rb->rbnext;  // Skip to point description info.
					rb = rb->rbnext;  // Skip to the pick location.
					pickpnt.set(rb->resval.rpoint[0],
						rb->resval.rpoint[1],
						rb->resval.rpoint[2]);

					rb = rb->rbnext;  // Will be normal vector if we're not in XY view.
									  // Otherwise, it'll be an RTLE for pick location.

					if (rb->restype == RT3DPOINT) {
						pickvec.set(rb->resval.rpoint[0],
							rb->resval.rpoint[1],
							rb->resval.rpoint[2]);
						rb = rb->rbnext;  // Make it point to the RTLE for the pick location.
					}
				}

				return CONTINUE;
			}

			class SSetLock {
				ads_name sset = {};
			public:
				~SSetLock() { acedSSFree(sset); }
				operator intptr_t * () { return sset; }
			};

			static inline void _p_select_a_line() {

				SSetLock sset;
				/*获取选择集*/
				if (RTNORM != acedSSGet(LR"(:S)", nullptr, nullptr, nullptr, sset)) {
					return;
				} 							

				int            sel_method;
				ads_name       subname;
				short          marker;
				AcGePoint3d    pickpnt;
				AcGeVector3d   pickvec;
				ads_name       ename;

				{
					/*获得选择集信息*/
					struct resbuf *rb;
					if (acedSSNameX(&rb, sset, 0) != RTNORM) {
						return;
					}

					if (!extractEntityInfo(rb,
						sel_method,
						ename,
						subname,
						marker,
						pickpnt,
						pickvec)) {
						return;
					}

				}

				AcDbObjectId polyId;
				acdbGetObjectId(polyId, ename);

				sstd::ArxClosePointer<AcDbEntity>  ent  ;
				if (acdbOpenAcDbEntity(ent, polyId, AcDb::kForRead) != Acad::eOk) {
					return;
				}

				union {
					AcDbBlockReference *  blRef;
				};

				blRef = AcDbBlockReference::cast(ent);

				if (blRef == nullptr) {

				}
				else {/*this is a 块*/
					struct resbuf *insStack;
					ads_point pickpoint;
					ads_matrix adsmat;

					pickpoint[0] = pickpnt[0];
					pickpoint[1] = pickpnt[1];
					pickpoint[2] = pickpnt[2];

					// Now get details on the entity that was
					// selected.
					// 
					if (acedNEntSelP(nullptr, 
						ename, 
						pickpoint, 
						TRUE,
						adsmat, 
						&insStack) != RTNORM){
						acutPrintf(L"\nFailure in acedNEntSelP");
						return;
					}

					AcDbObjectId objectId;
					acdbGetObjectId(objectId, ename);

					sstd::ArxClosePointer<AcDbEntity> ent2;
					if (acdbOpenAcDbEntity(ent2, objectId, AcDb::kForRead) != Acad::eOk) {
						return;
					}

					std::vector<AcDbObjectId> idArray;
					int count = 0;////////////////
					struct resbuf *rbIter = insStack;
					AcDbObjectId objId;
					acdbGetObjectId(objId, ename);
					idArray.push_back(objId);
					count++;

					while (rbIter != nullptr) {
						ename[0] = rbIter->resval.rlname[0];
						ename[1] = rbIter->resval.rlname[1];
						acdbGetObjectId(objId, ename);
						idArray.push_back(objId);
						count++;
						rbIter = rbIter->rbnext;
					}
					count--;

					acutRelRb(insStack);////////////////

					int                  numPaths;
					AcDbFullSubentPath  *subentPaths;
					AcGeMatrix3d         xform;
					if (Acad::eOk != blRef->getSubentPathsAtGsMarker(
						AcDb::kEdgeSubentType,
						marker,
						pickpnt,
						xform,
						numPaths,
						subentPaths,
						count,
						idArray.data())){
						acutPrintf(L"\nError obtaining subentity paths for Block Reference");
						return;
					}

					acutPrintf(L"\nHighlighting the first edge.");
					blRef->highlight(subentPaths[0]);
					pressEnterToContinue();
				    blRef->unhighlight(subentPaths[0]);

				}/*this is a 块*/

			}
		};
	}

	void SimpleSelect::main() {
		SelectALine::_p_select_a_line();
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

/*
http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html



*/


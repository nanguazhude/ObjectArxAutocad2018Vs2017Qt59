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

	void SimpleSelect::load() {//ACRX_CMD_TRANSPARENT
		//arx_add_main_command_usepickset<SimpleSelect>();
		acedRegCmds->addCommand(arx_group_name(),
			SimpleSelect::globalName(),
			SimpleSelect::localName(),
			ACRX_CMD_TRANSPARENT| ACRX_CMD_MODAL  ,
			&SimpleSelect::main
		);
	}

	namespace {

		static Acad::ErrorStatus
			getObjectAndGsMarker(AcDbObjectId& objId, AcGePoint3d& pt3d, ads_matrix& mx, resbuf*& rbChain, intptr_t& marker)
		{
			ads_name ename;
			ads_point pt;
			unsigned int uTransSpaceFlag = 1;

			// set uTransSpaceFlag to 0, if the current layout is in model space
			struct resbuf rb;
			acedGetVar(L"CVPORT", &rb);
			if (rb.resval.rint != 1)
				uTransSpaceFlag = 0; // Model space

									 // this function will give back the marker that is needed for subentity selection and the nesting entities of the 
									 // selected entity
			if (acedNEntSelPEx(L"\nPick edge : ", ename, pt, NULL, mx, &rbChain, uTransSpaceFlag, &marker) != RTNORM) {
				acutPrintf(L"\nacedNEntSelPEx has failed");
				return Acad::eInvalidAdsName;
			}

			pt3d.x = pt[0];
			pt3d.y = pt[1];
			pt3d.z = pt[2];

			acdbGetObjectId(objId, ename);

			return Acad::eOk;
		}

		static void
			highlightEdge(const AcDbObjectId& objId, AcGePoint3d& pt3d, ads_matrix& mx, resbuf*& rbChain, const int marker)
		{
			AcDbEntity *pSolid = NULL; // the solid object
			AcDbEntity* pTMBlockRef = NULL; // topmost blockreference - if there is one
			AcDbEntity* pEntHighlight = NULL; // it's either the solid or the block reference that contains it

											  // open the solid
			acdbOpenAcDbEntity(pSolid, objId, AcDb::kForRead);

			// Get the subentity ID for the edge that is picked
			//
			AcGeMatrix3d xform;
			int numIds;
			AcDbFullSubentPath* subentIds;

			xform.setCoordSystem(
				(const AcGePoint3d&)AcGePoint3d(mx[0][0], mx[0][1], mx[0][2]),
				(const AcGeVector3d&)AcGeVector3d(mx[1][0], mx[1][1], mx[1][2]),
				(const AcGeVector3d&)AcGeVector3d(mx[2][0], mx[2][1], mx[2][2]),
				(const AcGeVector3d&)AcGeVector3d(mx[3][0], mx[3][1], mx[3][2]));

			// count containing block references 
			int numInserts = 0;
			// should be one less than the total number of entries in entAndInsertStack 
			// because the first entry is the entity itself, which is not a BlockReference
			for (resbuf* currentInsert = rbChain; currentInsert != NULL; currentInsert = currentInsert->rbnext)
				numInserts++;

			Acad::ErrorStatus err;

			// it is inside a block 
			if (numInserts > 0)
			{
				// create an array
				AcDbObjectId* Inserts = new AcDbObjectId[numInserts + 1];
				Inserts[0] = objId;

				// we need to fill up the list going from the direct block reference that contains the solid 
				// to the topmost block reference
				int currNumber = 1;
				for (resbuf* current = rbChain; current != NULL; current = current->rbnext, currNumber++)
					acdbGetObjectId(Inserts[currNumber], current->resval.rlname);;

				// get topmost blockreference
				acdbOpenAcDbEntity(pTMBlockRef, Inserts[numInserts], AcDb::kForRead);

				// xform and pt3d are not used in case of AcDb3dSolid
				AcGePoint3d point;
				AcGeMatrix3d matrix;
				err = pTMBlockRef->getSubentPathsAtGsMarker(AcDb::kEdgeSubentType,
					marker, point, matrix, /*pt3d, xform, */numIds, subentIds, numInserts, Inserts);

				// as an extra check let's highlight the topmost blockreference as well
				pEntHighlight = pTMBlockRef;
			}
			else
			{
				// xform and pt3d are not used in case of AcDb3dSolid
				err = pSolid->getSubentPathsAtGsMarker(AcDb::kEdgeSubentType,
					marker, pt3d, xform, numIds, subentIds);

				pEntHighlight = pSolid;
			}

			// At this point the subentId's variable contains the
			// address of an array of AcDbFullSubentPath objects.
			// The array should be one element long, so the picked
			// edge's AcDbFullSubentPath is in subentIds[0].
			//
			// For objects with no edges (such as a sphere), the
			// code to highlight an edge is meaningless and must
			// be skipped.
			//
			if (numIds > 0)
			{
				ACHAR dummy[133]; // space for acedGetString pauses below

								  // Highlight the edge.
								  //
				pEntHighlight->highlight(subentIds[0]); // Highlights the edge on the solid in the right block
														// Pause to let user see the effect.
														//
				acedGetString(0, L"\nEdge is highlighted. Press <RETURN> to continue...",
					dummy);
				pEntHighlight->unhighlight(subentIds[0]);

				pEntHighlight->highlight(); // highlights the entire block reference - if the solid is inside one
											// Pause to let user see the effect.
											//
				acedGetString(0, L"\nTopmost nesting block reference is highlighted. Press <RETURN> to continue...",
					dummy);
				pEntHighlight->unhighlight();
			}

			// delete temporary array
			delete[]subentIds;

			// close opened entities
			pSolid->close();
			if (pTMBlockRef)
				pTMBlockRef->close();
		}

		void _p_select_a_line() {
			AcDbObjectId objId;
			AcGePoint3d pt3d;
			intptr_t marker;
			ads_matrix mx;
			resbuf* rbChain = NULL; // contains the nesting blockreferences

			if (getObjectAndGsMarker(objId, pt3d, mx, rbChain, marker) != Acad::eOk)
				return;

			highlightEdge(objId, pt3d, mx, rbChain, marker);

			ads_free(rbChain);
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

/*
http://adndevblog.typepad.com/autocad/2013/01/select-the-edge-of-a-nested-solid.html



*/


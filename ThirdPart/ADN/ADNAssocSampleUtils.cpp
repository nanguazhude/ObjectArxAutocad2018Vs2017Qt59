/** Dimension **/
#include "AdnAssocSampleUtils.hpp"
#include "AdnConstraintEvaluationCallback.hpp"
#include <set>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AdnAssocSampleUtils::AdnAssocSampleUtils(void)
{

}

AdnAssocSampleUtils::~AdnAssocSampleUtils(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AcDbObjectId AdnAssocSampleUtils::getConstraintGroup(
	const AcDbObjectId& spaceId,
	bool createIfDoesNotExist)
{
	AcDbObjectId idConstrGroup = AcDbObjectId::kNull;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

	if (networkId.isNull())
	{
		return AcDbObjectId::kNull;
	}

	// get the current UCS plane
	AcGeMatrix3d ucsMatrix;
	acedGetCurrentUCS(ucsMatrix);

	AcGePoint3d origin;
	AcGeVector3d xAxis, yAxis, zAxis;
	ucsMatrix.getCoordSystem(origin, xAxis, yAxis, zAxis);

	AcGePlane currentPlane(origin, xAxis, yAxis);

	// Try to find the constraint group in the associative network
	if (idConstrGroup.isNull() && createIfDoesNotExist)
	{
		AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForRead);

		if (pNetwork.openStatus() != Acad::eOk)
		{
			return AcDbObjectId::kNull;
		}

		const AcDbObjectIdArray& actionsInNetwork = pNetwork->getActions();

		for (int nCount = 0; nCount < actionsInNetwork.length(); ++nCount)
		{
			const AcDbObjectId& idAction = actionsInNetwork[nCount];

			if (idAction == AcDbObjectId::kNull)
				continue;

			if (actionsInNetwork[nCount].objectClass() == NULL ||
				!actionsInNetwork[nCount].objectClass()->isDerivedFrom(AcDbAssoc2dConstraintGroup::desc()))
				continue;

			AcDbObjectPointer<AcDbAssocAction> pAction(idAction, kForRead);

			if (pAction.openStatus() != Acad::eOk)
				continue;

			const AcDbAssoc2dConstraintGroup* const pConstGrp =
				static_cast<AcDbAssoc2dConstraintGroup*>(pAction.object());

			if (!pConstGrp)
				continue;

			if (currentPlane.isCoplanarTo(pConstGrp->getWorkPlane()))
			{
				// We have an constraint group which is built on same UCS
				return idAction;
			}
		}
	}

	if (idConstrGroup.isNull() && createIfDoesNotExist)
	{
		AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);

		if (pNetwork.openStatus() != Acad::eOk)
		{
			return AcDbObjectId::kNull;
		}

		AcDbAssoc2dConstraintGroup* const pConstrGroup = new AcDbAssoc2dConstraintGroup(currentPlane);

		AcDbDatabase *pDatabase = pNetwork->database();

		if (pDatabase->addAcDbObject(idConstrGroup, pConstrGroup) != Acad::eOk)
		{
			delete pConstrGroup;
			return AcDbObjectId::kNull;
		}

		pConstrGroup->close();

		if (pNetwork->addAction(idConstrGroup, true) != Acad::eOk)
		{
			return AcDbObjectId::kNull;
		}
	}

	return idConstrGroup;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getFullSubentPaths(
	const AcDbEntity *pEnt,
	AcDb::SubentType subentType,
	AcDbFullSubentPathArray &aPaths)
{
	ErrorStatus es;
	//AcConstrainedGeometry* pConsGeom = NULL;

	// Get the Protocol extension associated with the entity
	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE =
		AcDbAssocPersSubentIdPE::cast(pEnt->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcArray<AcDbSubentId> subentIds;

	// Get all the subentities associated with the subentity type such as the vertices or edges
	if ((es = pAssocPersSubentIdPE->getAllSubentities(pEnt, subentType, subentIds)) != Acad::eOk)
		return es;

	//Get subent path and put them into aPaths collection
	for (int i = 0; i < subentIds.length(); ++i)
	{
		aPaths.append(AcDbFullSubentPath(pEnt->objectId(), subentIds[i]));
	}

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::addConstrainedGeometry(
	AcDbFullSubentPathArray &aPaths,
	AcArray<AcConstrainedGeometry*> &pConsGeoms)
{
	ErrorStatus es;

	AcDbDatabase *pDatabase = acdbHostApplicationServices()->workingDatabase();

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(
		pDatabase->currentSpaceId(),
		true);

	if (consGrpId.isNull())
		return Acad::eInvalidInput;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);

	// Could not open constraint group for updates.
	if ((es = p2dConstrGrp.openStatus()) != Acad::eOk)
		return es;

	//Add the relevent subentities as the constrained geometry
	for (int i = 0; i < aPaths.length(); ++i)
	{
		AcConstrainedGeometry* pConsGeom = NULL;

		es = p2dConstrGrp->addConstrainedGeometry(aPaths[i], pConsGeom);

		if (!((es == Acad::eAlreadyInGroup) || (es == Acad::eOk)))
			return es;

		pConsGeoms.append(pConsGeom);
	}

	return Acad::eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::addGeomConstraint(
	const AcGeomConstraint::GeomConstraintType& constraintType,
	AcArray<AcConstrainedGeometry*> &pConsGeoms,
	const AcGePoint3dArray& points)
{
	ErrorStatus es;

	// Get the 2D constraint Group
	AcDbDatabase *pDatabase = acdbHostApplicationServices()->workingDatabase();

	const AcString dictionaryKey(DICTIONARYKEY);

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(
		pDatabase->currentSpaceId(),
		true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);


	AcGeMatrix3d coordSysInv(AdnAssocSampleUtils::getConstraintGroupCoordSysInv(p2dConstrGrp));

	//Get closest constrained points

	if (pConsGeoms.length() != points.length())
		return Acad::eInvalidInput;

	AcArray<AcConstrainedGeometry*> pConsPoints;

	for (int i = 0; i < pConsGeoms.length(); ++i)
	{
		AcGePoint3d entPtInConstGrp = points[i];
		entPtInConstGrp.transformBy(coordSysInv);

		AcConstrainedPoint* consPoint;

		if ((es = AdnAssocSampleUtils::getClosestConstrainedPoint(pConsGeoms[i], entPtInConstGrp, consPoint)) != Acad::eOk)
			return es;

		pConsPoints.append(consPoint);
	}

	// Add the constraint to the selected entities
	AcGeomConstraint **ppNewConstraint = NULL;

	if ((es = p2dConstrGrp->addGeometricalConstraint(constraintType, pConsPoints, ppNewConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId idNetwork =
		AcDbAssocNetwork::getInstanceFromDatabase(pDatabase,
			true,
			dictionaryKey);

	if (idNetwork.isNull())
		return Acad::eInvalidInput;

	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(idNetwork, kForWrite);

	if (pNetwork.openStatus() != Acad::eOk)
		return Acad::eInvalidInput;

	// Evaluate the constraint. 
	// This is like a refresh to make sure the constraint takes effect.
	AdnConstraintEvaluationCallback callback;

	pNetwork->evaluate(&callback);

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::addGeomConstraint(
	const AcGeomConstraint::GeomConstraintType& constraintType,
	AcDbFullSubentPathArray &aPaths)
{
	ErrorStatus es;

	// Get the 2D constraint Group
	AcDbDatabase *pDatabase = acdbHostApplicationServices()->workingDatabase();
	const AcString dictionaryKey(DICTIONARYKEY);

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(
		pDatabase->currentSpaceId(),
		true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(
		consGrpId, kForWrite);

	// Add the constraint to the selected entities
	AcGeomConstraint **ppNewConstraint = NULL;

	if ((es = p2dConstrGrp->addGeometricalConstraint(
		constraintType, aPaths, ppNewConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId idNetwork =
		AcDbAssocNetwork::getInstanceFromDatabase(pDatabase,
			true,
			dictionaryKey);

	if (idNetwork.isNull())
		return Acad::eInvalidInput;

	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(idNetwork, kForWrite);

	if (pNetwork.openStatus() != Acad::eOk)
		return Acad::eInvalidInput;

	// Evaluate the constraint. 
	// This is like a refresh to make sure the constraint takes effect.
	AdnConstraintEvaluationCallback callback;

	pNetwork->evaluate(&callback);

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::addGeomConstraint(
	const AcGeomConstraint::GeomConstraintType& constraintType,
	AcDbFullSubentPathArray &aPaths,
	AcDbFullSubentPathArray &fixedGeom)
{
	ErrorStatus es;

	// Get the 2D constraint Group
	AcDbDatabase *pDatabase = acdbHostApplicationServices()->workingDatabase();
	const AcString dictionaryKey(DICTIONARYKEY);

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(
		pDatabase->currentSpaceId(),
		true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(
		consGrpId, kForWrite);

	// Add the constraint to the selected entities
	AcGeomConstraint **ppNewConstraint = NULL;

	if ((es = p2dConstrGrp->addGeometricalConstraint(
		constraintType, aPaths, ppNewConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId idNetwork =
		AcDbAssocNetwork::getInstanceFromDatabase(pDatabase,
			true,
			dictionaryKey);

	if (idNetwork.isNull())
		return Acad::eInvalidInput;

	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(idNetwork, kForWrite);

	if (pNetwork.openStatus() != Acad::eOk)
		return Acad::eInvalidInput;

	// Evaluate the constraint. 
	// This is like a refresh to make sure the constraint takes effect.
	AdnConstraintEvaluationCallback callback;

	callback.addTempFixedSubEnts(fixedGeom);

	pNetwork->evaluate(&callback);

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestEdgeSubEntPath(
	const AcDbObjectId& entId,
	const AcGePoint3d& entPt,
	AcDbFullSubentPath &closestPath,
	double* pMinDist,
	AcGePoint3d* pClosestPt)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbEntityPointer pEnt(entId, AcDb::kForRead);

	if ((es = pEnt.openStatus()) != Acad::eOk)
		return es;

	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		return getClosestEdgeSubEntPathBref(entId, entPt, closestPath, pMinDist);
	}

	// Get the Protocol extension associated with the entity
	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE =
		AcDbAssocPersSubentIdPE::cast(pEnt->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcDbFullSubentPathArray aPathsTemp;
	AdnAssocSampleUtils::getFullSubentPaths(pEnt, AcDb::kEdgeSubentType, aPathsTemp);

	if (aPathsTemp.length() == 0)
		return Acad::eInvalidInput;

	double minDist = -1.0;

	for (int i = 0; i < aPathsTemp.length(); ++i)
	{
		AcGeCurve3d *edgeCurve = NULL;

		if ((es = pAssocPersSubentIdPE->getEdgeSubentityGeometry(
			pEnt, aPathsTemp[i].subentId(), edgeCurve)) != Acad::eOk)
			return es;

		double dist = edgeCurve->distanceTo(entPt);

		if (dist < minDist || minDist < 0.0)
		{
			minDist = dist;
			closestPath = aPathsTemp[i];

			if (pClosestPt)
			{
				*pClosestPt = edgeCurve->closestPointTo(entPt);
			}
		}

		delete edgeCurve;
	}

	// Need output min distance for 
	// AdnAssocSampleUtils::getClosestEdgeSubEntPathBref
	if (pMinDist)
	{
		*pMinDist = minDist;
	}

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestEdgeSubEntPathBref(
	const AcDbObjectId& brefId,
	const AcGePoint3d& entPt,
	AcDbFullSubentPath &closestPath,
	double* pMinDist)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbObjectPointer<AcDbBlockReference> pBref(brefId, kForRead);

	if ((es = pBref.openStatus()) != Acad::eOk)
		return es;

	AcDbObjectPointer<AcDbBlockTableRecord> pBtr(pBref->blockTableRecord(), kForRead);

	if ((es = pBtr.openStatus()) != Acad::eOk)
		return es;

	AcDbBlockTableRecordIterator* pBtrIterator = NULL;

	if ((es = pBtr->newIterator(pBtrIterator)) != Acad::eOk)
		return es;

	AcDbFullSubentPath closestPathInBlock;

	AcGePoint3d blockPoint(entPt);
	blockPoint.transformBy(pBref->blockTransform().inverse());

	double minDist = -1.0;

	//Find the closest edge path in block
	for (pBtrIterator->start(); !pBtrIterator->done(); pBtrIterator->step())
	{
		AcDbObjectId nestedId;
		pBtrIterator->getEntityId(nestedId);

		double dist;
		AcDbFullSubentPath subEntPath;

		if (AdnAssocSampleUtils::getClosestEdgeSubEntPath(
			nestedId, blockPoint, subEntPath, &dist) != Acad::eOk)
			continue;

		if (dist < minDist || minDist < 0)
		{
			minDist = dist;
			closestPathInBlock = subEntPath;
		}
	}

	AcDbObjectIdArray idArray;
	idArray.append(brefId);
	idArray.append(closestPathInBlock.objectIds());
	closestPath = AcDbFullSubentPath(idArray, closestPathInBlock.subentId());

	if (pMinDist)
	{
		*pMinDist = minDist;
	}

	delete pBtrIterator;

	if (minDist < 0)
		return Acad::eInvalidInput;

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestVertexInfo(
	const AcDbObjectId& entId,
	const AcDbFullSubentPath& edgeSubentPath,
	const AcGePoint3d& pt,
	AcGePoint3d& closestVertexPos,
	AcDbFullSubentPath& closestVertexSubentPath)
{
	Acad::ErrorStatus es;

	AcDbSmartObjectPointer <AcDbEntity> pEntity(entId, AcDb::kForRead);

	if ((es = pEntity.openStatus()) != Acad::eOk)
		return es;

	if (pEntity->isKindOf(AcDbBlockReference::desc()))
	{
		return AdnAssocSampleUtils::getClosestVertexInfoBref(entId,
			edgeSubentPath,
			pt,
			closestVertexPos,
			closestVertexSubentPath);
	}

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE =
		AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcArray<AcDbSubentId> vertexSubentIds;

	if (pEntity->isKindOf(AcDbSpline::desc()))
	{
		AcDbSubentId startVertexSubentId;
		AcDbSubentId endVertexSubentId;

		AcArray<AcDbSubentId> controlPointSubentIds;
		AcArray<AcDbSubentId> fitPointSubentIds;

		if ((es = pAssocPersSubentIdPE->getSplineEdgeVertexSubentities(pEntity,
			AcDbSubentId(),
			startVertexSubentId,
			endVertexSubentId,
			controlPointSubentIds,
			fitPointSubentIds)) != Acad::eOk)
			return es;

		vertexSubentIds.append(startVertexSubentId);
		vertexSubentIds.append(endVertexSubentId);
		vertexSubentIds.append(controlPointSubentIds);
	}
	else
	{
		AcDbSubentId startVertexSubentId;
		AcDbSubentId endVertexSubentId;

		if ((es = pAssocPersSubentIdPE->getEdgeVertexSubentities(pEntity,
			edgeSubentPath.subentId(),
			startVertexSubentId,
			endVertexSubentId,
			vertexSubentIds)) != Acad::eOk)
			return es;

		vertexSubentIds.append(startVertexSubentId);
		vertexSubentIds.append(endVertexSubentId);
	}

	double minDist = -1.0;

	AcDbSubentId closestId;

	for (int i = 0; i < vertexSubentIds.length(); ++i)
	{
		AcGePoint3d vertexPos;

		if ((es = pAssocPersSubentIdPE->getVertexSubentityGeometry(
			pEntity,
			vertexSubentIds[i],
			vertexPos)) != eOk)
			return es;

		double dist = vertexPos.distanceTo(pt);

		if (minDist < 0 || dist < minDist)
		{
			minDist = dist;

			closestId = vertexSubentIds[i];

			closestVertexPos = vertexPos;
		}
	}

	closestVertexSubentPath = AcDbFullSubentPath(entId, closestId);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use: 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestVertexPos(
	const AcDbObjectId& entId,
	const AcDbFullSubentPath& edgeSubentId,
	const AcGePoint3d& pt,
	AcGePoint3d& closestVertexPos)
{
	AcDbFullSubentPath vertexPath;

	return AdnAssocSampleUtils::getClosestVertexInfo(
		entId,
		edgeSubentId,
		pt,
		closestVertexPos,
		vertexPath);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestVertexInfoBref(
	const AcDbObjectId& brefId,
	const AcDbFullSubentPath& edgeSubentPath,
	const AcGePoint3d& pt,
	AcGePoint3d& closestVertexPos,
	AcDbFullSubentPath& closestVertexSubentPath)
{
	Acad::ErrorStatus es = Acad::eOk;

	int len = edgeSubentPath.objectIds().length();

	AcDbObjectId lastBrefId = edgeSubentPath.objectIds()[len - 2];

	AcDbObjectPointer<AcDbBlockReference> pBref(lastBrefId, kForRead);

	if ((es = pBref.openStatus()) != Acad::eOk)
		return es;

	AcDbObjectPointer<AcDbBlockTableRecord> pBtr(pBref->blockTableRecord(), kForRead);

	if ((es = pBtr.openStatus()) != Acad::eOk)
		return es;

	AcDbBlockTableRecordIterator* pBtrIterator = NULL;

	if ((es = pBtr->newIterator(pBtrIterator)) != Acad::eOk)
		return es;

	AcDbObjectId targetId = edgeSubentPath.objectIds()[len - 1];

	AcDbCompoundObjectId compoundId = AcDbCompoundObjectId(
		targetId, edgeSubentPath.objectIds(), NULL);

	AcGeMatrix3d compoundTransfo;
	compoundId.getTransform(compoundTransfo);

	AcGePoint3d blockPoint(pt);
	blockPoint.transformBy(compoundTransfo.inverse());

	AcGePoint3d closestVertexPosInBlock;
	AcDbFullSubentPath closestVertexSubentPathInBlock;

	for (pBtrIterator->start(); !pBtrIterator->done(); pBtrIterator->step())
	{
		AcDbObjectId nestedId;
		pBtrIterator->getEntityId(nestedId);

		if (nestedId != targetId)
			continue;

		AcDbFullSubentPath edgeSubentPathBtr(targetId, edgeSubentPath.subentId());

		if ((es = AdnAssocSampleUtils::getClosestVertexInfo(targetId,
			edgeSubentPathBtr,
			blockPoint,
			closestVertexPosInBlock,
			closestVertexSubentPathInBlock)) != Acad::eOk)

			return es;
	}

	closestVertexPos = closestVertexPosInBlock;
	closestVertexPos.transformBy(compoundTransfo);

	AcDbObjectIdArray idArray;

	idArray.append(brefId);
	idArray.append(closestVertexSubentPathInBlock.objectIds());

	closestVertexSubentPath = AcDbFullSubentPath(
		idArray,
		closestVertexSubentPathInBlock.subentId());

	return es;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The test this method is doing may not be entirely accurate especially differentiating between line and axis. Definately need
// to revisit this and bring it in line with Acad impl
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AdnAssocSampleUtils::isEntityValidForConstraint(
	const AcDbObjectId& entId,
	const AcGePoint3d& entPt,
	AcGeomConstraint::GeomConstraintType constraintType)
{
	Acad::ErrorStatus es;

	//bool isValid = false;

	AcDbFullSubentPath edgeSubent;

	if ((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, entPt, edgeSubent)) != Acad::eOk)
		return false;

	AcGeCurve3d* pCurve = AdnAssocSampleUtils::getCurveForEdgeSubentity(entId, edgeSubent);

	if (!pCurve)
		return false;

	AcGe::EntityId curveType = pCurve->type();

	delete pCurve;

	switch (constraintType)
	{
	case AcGeomConstraint::kConcentric:

		if (curveType == AcGe::kCircArc3d ||
			curveType == AcGe::kEllipArc3d)
		{
			return true;
		}
		break;

	case AcGeomConstraint::kHorizontal:
	case AcGeomConstraint::kVertical:
	case AcGeomConstraint::kParallel:
	case AcGeomConstraint::kPerpendicular:
	case AcGeomConstraint::kColinear:

		if (curveType == AcGe::kLinearEnt3d ||
			curveType == AcGe::kLineSeg3d ||
			curveType == AcGe::kRay3d ||
			curveType == AcGe::kLine3d)
		{
			return true;
		}
		break;

	case AcGeomConstraint::kCoincident:
	case AcGeomConstraint::kFix:
	case AcGeomConstraint::kSymmetric:

		if (curveType == AcGe::kCircArc3d ||
			curveType == AcGe::kEllipArc3d ||
			curveType == AcGe::kLinearEnt3d ||
			curveType == AcGe::kLineSeg3d ||
			curveType == AcGe::kRay3d ||
			curveType == AcGe::kLine3d ||
			curveType == AcGe::kNurbCurve3d)
		{
			return true;
		}
		break;

	case AcGeomConstraint::kTangent:

		if (curveType == AcGe::kCircArc3d ||
			curveType == AcGe::kEllipArc3d ||
			curveType == AcGe::kLinearEnt3d ||
			curveType == AcGe::kLineSeg3d ||
			curveType == AcGe::kRay3d ||
			curveType == AcGe::kLine3d)
		{
			return true;
		}
		break;

	case AcGeomConstraint::kEqualLength:
	case AcGeomConstraint::kEqualRadius:

		if (curveType == AcGe::kLinearEnt3d ||
			curveType == AcGe::kLineSeg3d ||
			curveType == AcGe::kLine3d ||
			curveType == AcGe::kCircArc3d)
		{
			return true;
		}
		break;

	case AcGeomConstraint::kG2Smooth:

		if (curveType == AcGe::kSplineEnt3d ||
			curveType == AcGe::kCircArc3d ||
			curveType == AcGe::kLinearEnt3d ||
			curveType == AcGe::kLineSeg3d ||
			curveType == AcGe::kLine3d ||
			curveType == AcGe::kNurbCurve3d)
		{
			return true;
		}
		break;

	default: break;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AcGeCurve3d* AdnAssocSampleUtils::getCurveForEdgeSubentity(const AcDbObjectId& entId,
	const AcDbFullSubentPath& edgeSubent)
{
	AcDbEntityPointer pEntity(entId, AcDb::kForRead);

	if (pEntity.openStatus() != Acad::eOk)
		return NULL;

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE
		= AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcGeCurve3d* pCurve = NULL;

	if (pAssocPersSubentIdPE->getEdgeSubentityGeometry(pEntity.object(), edgeSubent.subentId(), pCurve) != Acad::eOk)
		return NULL;

	return pCurve;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getValidVertexSubentities(const AcDbEntity* pEntity,
	const AcDbSubentId& edgeSubentId,
	AcArray<AcDbSubentId>& vertexSubentIds,
	AcArray<AcDbSubentId>& fixedVertexSubentIds)
{
	Acad::ErrorStatus es;

	vertexSubentIds.setLogicalLength(0);
	fixedVertexSubentIds.setLogicalLength(0);

	// validate the input values
	if (!pEntity)
		return Acad::eInvalidInput;

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE
		= AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	if (!pAssocPersSubentIdPE)
		return Acad::eInvalidInput;

	if (pEntity->isKindOf(AcDbSpline::desc()))
	{
		AcDbSubentId startVertexSubentId, endVertexSubentId;
		AcArray<AcDbSubentId> controlPointSubentIds, fitPointSubentIds;

		if ((es = pAssocPersSubentIdPE->getSplineEdgeVertexSubentities(pEntity,
			AcDbSubentId(),
			startVertexSubentId,
			endVertexSubentId,
			controlPointSubentIds,
			fitPointSubentIds)) != Acad::eOk)
		{
			vertexSubentIds.setLogicalLength(0);
			fixedVertexSubentIds.setLogicalLength(0);
			return es;
		}

		const int length = controlPointSubentIds.length();

		// There should be at least 2 valid control points. 
		//For spline, we are going to support only start and end control points
		if (length < 2)
			return Acad::eInvalidInput;;

		AcDbSubentId startControlPointSubentId = controlPointSubentIds[0];
		AcDbSubentId endControlPointSubentId = controlPointSubentIds[length - 1];
		vertexSubentIds.append(startControlPointSubentId);
		vertexSubentIds.append(endControlPointSubentId);
		fixedVertexSubentIds.append(vertexSubentIds);

		return es;
	}
	else if (edgeSubentId != kNullSubentId)
	{
		AcDbSubentId startVertexSubentId;
		AcDbSubentId endVertexSubentId;
		AcArray<AcDbSubentId> otherVertexSubentIds;

		if ((es = pAssocPersSubentIdPE->getEdgeVertexSubentities(pEntity,
			edgeSubentId,
			startVertexSubentId,
			endVertexSubentId,
			otherVertexSubentIds)) != Acad::eOk)
		{
			vertexSubentIds.setLogicalLength(0);
			fixedVertexSubentIds.setLogicalLength(0);
			return es;
		}

		if (startVertexSubentId != kNullSubentId)
			fixedVertexSubentIds.append(startVertexSubentId);

		if (endVertexSubentId != kNullSubentId)
			fixedVertexSubentIds.append(endVertexSubentId);

		vertexSubentIds.append(fixedVertexSubentIds);
		vertexSubentIds.append(otherVertexSubentIds);

		return es;
	}
	else
	{
		if ((es = pAssocPersSubentIdPE->getAllSubentities(pEntity,
			AcDb::kVertexSubentType,
			vertexSubentIds)) != Acad::eOk)
		{
			vertexSubentIds.setLogicalLength(0);
			fixedVertexSubentIds.setLogicalLength(0);
			return es;
		}

		return es;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AdnAssocSampleUtils::isInsertionPointBasedEntity(const AcDbObjectId& entityId)
{
	AcDbSmartObjectPointer <AcDbEntity> pEntity(entityId, AcDb::kForRead);

	if (pEntity.openStatus() != Acad::eOk)
		return false;

	return (pEntity &&
		(pEntity->isKindOf(AcDbBlockReference::desc()) ||
			pEntity->isKindOf(AcDbMText::desc()) ||
			pEntity->isKindOf(AcDbText::desc()) ||
			pEntity->isKindOf(AcDbAttributeDefinition::desc()) ||
			pEntity->isKindOf(AcDbAttribute::desc())));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AcGeMatrix3d UcsMatrix()
{
	AcGeMatrix3d ucs;
	acedGetCurrentUCS(ucs);
	return ucs;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::postToDatabase(AcDbEntity* pEntity,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	AcDbBlockTablePointer blkTable(pDb, AcDb::kForRead);

	if (blkTable.openStatus() != Acad::eOk)
		return blkTable.openStatus();

	if (!blkTable->has(spaceId))
		return Acad::eInvalidInput;

	AcDbBlockTableRecordPointer btr(spaceId, AcDb::kForWrite);

	if (btr.openStatus() != Acad::eOk)
		return btr.openStatus();

	return btr->appendAcDbEntity(pEntity);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdnAssocSampleUtils::doCleanUp(AcDbObjectIdArray& cleanupObjectIds)
{
	for (int i = 0; i < cleanupObjectIds.length(); ++i)
	{
		AcDbSmartObjectPointer<AcDbObject> pObject(cleanupObjectIds[i], kForWrite);

		if (pObject.openStatus() == Acad::eOk)
			pObject->erase(true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::createAlignedDim(const AcGePoint3d& entPt1,
	const AcGePoint3d& entPt2,
	const AcGePoint3d& dimPos,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimPt1(entPt1);
	AcGePoint3d dimPt2(entPt2);
	AcGePoint3d dimTxt(dimPos);
	/******************************************************/
	AcDbAlignedDimension* dim = new AcDbAlignedDimension();

	dim->setDatabaseDefaults();

	dim->setXLine1Point(dimPt1.transformBy(ucsMatrixInv));
	dim->setXLine2Point(dimPt2.transformBy(ucsMatrixInv));
	dim->setTextPosition(dimTxt.transformBy(ucsMatrixInv));

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		dim->setDimensionStyle(pDb->textstyle());
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	dim->close();

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getLinearEdgeData(const AcDbObjectId& entId,
	const AcGePoint3d& entPt,
	AcDbFullSubentPath& edgeEntPath,
	AcGePoint3d& startPoint,
	AcGePoint3d& endPoint)
{
	Acad::ErrorStatus es;

	if ((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, entPt, edgeEntPath)) != Acad::eOk)
		return es;

	AcDbSmartObjectPointer <AcDbEntity> pEntity(entId, AcDb::kForRead);

	if ((es = pEntity.openStatus()) != Acad::eOk)
		return es;

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE =
		AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcGeCurve3d* segment = NULL;
	if ((es = pAssocPersSubentIdPE->getEdgeSubentityGeometry(pEntity, edgeEntPath.subentId(), segment)) != Acad::eOk)
		return es;

	if (!(segment->type() == AcGe::kLineSeg3d || segment->type() == AcGe::kLinearEnt3d))
		return Acad::eInvalidInput;

	if (!segment->hasStartPoint(startPoint))
	{
		delete segment;
		return Acad::eInvalidInput;
	}

	if (!segment->hasEndPoint(endPoint))
	{
		delete segment;
		return Acad::eInvalidInput;
	}

	delete segment;
	return Acad::eOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::create2LinesAngularDim(const AcGePoint3d& startPoint1,
	const AcGePoint3d& endPoint1,
	const AcGePoint3d& startPoint2,
	const AcGePoint3d& endPoint2,
	const AcGePoint3d& dimPos,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimStart1(startPoint1);
	AcGePoint3d dimEnd1(endPoint1);

	AcGePoint3d dimStart2(startPoint2);
	AcGePoint3d dimEnd2(endPoint2);

	AcGePoint3d dimTxt(dimPos);

	AcDb2LineAngularDimension* dim = new AcDb2LineAngularDimension(dimStart1.transformBy(ucsMatrixInv),
		dimEnd1.transformBy(ucsMatrixInv),
		dimStart2.transformBy(ucsMatrixInv),
		dimEnd2.transformBy(ucsMatrixInv),
		dimTxt.transformBy(ucsMatrixInv));

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	//dim->setConstraintDynamic(true);
	//dim->setDynamicDimension(true);

	dim->close();
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::createArcAngularDim(const AcGePoint3d& centerPoint,
	const AcGePoint3d& xLine1Point,
	const AcGePoint3d& xLine2Point,
	const AcGePoint3d& arcPoint,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimCenter(centerPoint);
	AcGePoint3d dimxLine1(xLine2Point);
	AcGePoint3d dimxLine2(xLine1Point);
	AcGePoint3d dimArc(arcPoint);

	AcDbArcDimension* dim = new AcDbArcDimension(dimCenter.transformBy(ucsMatrixInv),
		dimxLine1.transformBy(ucsMatrixInv),
		dimxLine2.transformBy(ucsMatrixInv),
		dimArc.transformBy(ucsMatrixInv));

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	dim->close();

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::create3PointAngularDim(const AcGePoint3d& centerPoint,
	const AcGePoint3d& xLine1Point,
	const AcGePoint3d& xLine2Point,
	const AcGePoint3d& arcPoint,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimCenter(centerPoint);
	AcGePoint3d dimxLine1(xLine2Point);
	AcGePoint3d dimxLine2(xLine1Point);
	AcGePoint3d dimArc(arcPoint);

	AcDb3PointAngularDimension* dim = new AcDb3PointAngularDimension(dimCenter.transformBy(ucsMatrixInv),
		dimxLine1.transformBy(ucsMatrixInv),
		dimxLine2.transformBy(ucsMatrixInv),
		dimArc.transformBy(ucsMatrixInv));

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	dim->close();

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::createRadialDim(const AcDbObjectId& /*entId*/,
	const AcGePoint3d& centerPt,
	const AcGePoint3d& chordPt,
	const double leaderLength,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimCenter(centerPt);
	AcGePoint3d dimChord(chordPt);

	AcDbRadialDimension* dim = new AcDbRadialDimension(dimCenter.transformBy(ucsMatrixInv),
		dimChord.transformBy(ucsMatrixInv),
		leaderLength);

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	dim->close();

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::createDiametricDim(const AcDbObjectId& /*entId*/,
	const AcGePoint3d& chordPt,
	const AcGePoint3d& farChordPt,
	const AcGePoint3d& dimPos,
	const double leaderLength,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimChord(chordPt);
	AcGePoint3d dimFarChord(farChordPt);
	AcGePoint3d dimTxt(dimPos);

	AcDbDiametricDimension* dim = new AcDbDiametricDimension(dimChord.transformBy(ucsMatrixInv),
		dimFarChord.transformBy(ucsMatrixInv),
		leaderLength);

	dim->useSetTextPosition();
	dim->setTextPosition(dimTxt.transformBy(ucsMatrixInv));

	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId);

	dimId = dim->objectId();
	dim->close();

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::createRotatedDim(const AcDbObjectId& /*entId1*/,
	const AcGePoint3d& entPt1,
	const AcDbFullSubentPath& /*edgeEntPath1*/,
	const AcDbObjectId& /*entId2*/,
	const AcGePoint3d& entPt2,
	const AcDbFullSubentPath& /*edgeEntPath2*/,
	const AcGePoint3d& dimPos,
	double rotation,
	AcDbObjectId& dimId,
	const AcDbObjectId& dimStyleId,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId)
{
	Acad::ErrorStatus es;

	AcGeMatrix3d ucsMatrix(UcsMatrix());
	AcGeMatrix3d ucsMatrixInv(ucsMatrix.inverse());

	AcGePoint3d dimPt1(entPt1);
	AcGePoint3d dimPt2(entPt2);
	AcGePoint3d dimTxt(dimPos);

	AcDbRotatedDimension* dim = new AcDbRotatedDimension(rotation,
		dimPt1.transformBy(ucsMatrixInv),
		dimPt2.transformBy(ucsMatrixInv),
		dimTxt.transformBy(ucsMatrixInv));

	dim->setDatabaseDefaults();
	dim->transformBy(ucsMatrix);

	if (dimStyleId != AcDbObjectId::kNull)
	{
		dim->setDimensionStyle(dimStyleId);
	}
	else
	{
		AcDbDimStyleTableRecord* pDimStyle = new AcDbDimStyleTableRecord;
		pDb->getDimstyleData(pDimStyle);
		pDimStyle->setDimscale(1.0);
		dim->setDimstyleData(pDimStyle);
		delete pDimStyle;
	}

	if ((es = AdnAssocSampleUtils::postToDatabase(dim, pDb, spaceId)) == Acad::eOk)
	{
		dimId = dim->objectId();
		dim->close();
	}

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::getNameAndExpressionFromDimension(const AcDbObjectId& networkId,
	const AcDbObjectId& dimId,
	const AcDbAssocConstraintType& constraintType,
	AcString &name,
	AcString &expression)
{
	Acad::ErrorStatus err = eOk;

	AcString namePrefix/*前缀*/;
	AcString dfltName/*   */;
	AcString dfltExpression/*  */;

	name.setEmpty();
	expression.setEmpty();

	// Choose the name prefix based on the type of constraint
	switch (constraintType)
	{
	case kDistanceAssocConstraintType:
	case kHorizontalDistanceAssocConstraintType:
	case kVerticalDistanceAssocConstraintType:
		namePrefix = ACRX_T("wddd_");
		break;

	case kAngle0AssocConstraintType:
	case kAngle1AssocConstraintType:
	case kAngle2AssocConstraintType:
	case kAngle3AssocConstraintType:
		namePrefix = ACRX_T("xang_");
		break;

	case kRadiusAssocConstraintType:
		namePrefix = ACRX_T("yrad_");
		break;

	case kDiameterAssocConstraintType:
		namePrefix = ACRX_T("zdia_");
		break;

	default:
		assert(!ACRX_T("Unsupported dimensional constraint type"));
		namePrefix = ACRX_T("error");
		break;
	}

	AcString dimensionText;
	double measurement = 0.0;
	bool isAngular = false;

	{
		AcDbObjectPointer<AcDbDimension> pDim(dimId, kForRead);

		if ((err = pDim.openStatus()) != eOk)
			return err;

		if (pDim->isConstraintObject())
		{
			LPTSTR pDimText = pDim->dimensionText();
			if (pDimText)
			{
				if (*pDimText) {
					dimensionText = pDimText;
				}
				free(pDimText);
			}
		}

		if ((err = pDim->measurement(measurement)) != Acad::eOk)
			return err;

		isAngular = pDim->isKindOf(AcDb2LineAngularDimension::desc()) || pDim->isKindOf(AcDb3PointAngularDimension::desc());
	}

	// Extract the dimension name and expression given the entity text
	err = AcDbAssocDimDependencyBodyBase::getNameAndExpressionFromEntityText(dimensionText,
		true,
		measurement,
		isAngular,
		dfltName,
		dfltExpression);

	assert(!dfltExpression.isEmpty());

	//default name should be empty if it's not a dimconstraint, need to generate a unique name from prefix
	if (dfltName.isEmpty())
	{
		AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForRead);

		if (pNetwork.openStatus() != eOk)
		{
			assert(!ACRX_T("Cannot open an Assoc Network"));
			return eInvalidInput;
		}

		AcDbAssocNetworkIterator iter(pNetwork);
		std::set<int> suffixArray;

		/*
		生成一个名字
		*/
		while (iter.moveNext()) {

			AcDbObjectPointer<AcDbAssocVariable> pVar(iter.current(), kForRead, true);

			if (pVar.openStatus() != eOk) {
				continue;
			}

			const AcString varName = pVar->name();

			if (varName.matchNoCase(namePrefix) == namePrefix.length()) {
				const AcString number = varName.substr(namePrefix.length(), -1);
				int val = number.asDeci(10)/*将字符串转为十进制数字*/;
				if (val >= 0) {
					suffixArray.insert(val);
				}
			}
		}

		int index = 1;
		do {/*给index一个初始值*/
			if (false == suffixArray.empty()) {
				if (static_cast<int>(suffixArray.size()) == (*suffixArray.rbegin())) {
					index = (*suffixArray.rbegin()) + 1;
					break;
				}
				auto var1 = suffixArray.begin();
				if ((*var1) > 1) { break; }
				auto var0 = var1++;
				const auto varE = suffixArray.end();
				for (; var1 != varE; var0 = var1++) {
					if (((*var1) - (*var0)) < 2) { continue; }
					break;
				}
				index = (*var0) + 1;
			}
		} while (false);

		while (suffixArray.count(index) > 0) {
			if (index < 0) {
				assert(!"Interger overflow");
				return eInvalidInput;
			}
			++index;
		}

		dfltName = namePrefix;

		const auto buf = std::to_wstring(index);
		switch (buf.size()) {
		case 1:dfltName += L"00"; break;
		case 2:dfltName += L"0"; break;
		//case 3:dfltName += L"000"; break;
		//case 4:dfltName += L"00"; break;
		//case 5:dfltName += L"0"; break;
		}
		dfltName += buf.c_str();
	}

	name = dfltName;
	expression = dfltExpression;

	return err;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::addNewVariableToAssocNetwork(const AcDbObjectId& networkId,
	const AcString& name,
	const AcString& expression,
	AcDbObjectId& varId)
{
	Acad::ErrorStatus es = eOk;
	varId = AcDbObjectId::kNull;

	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);

	if ((es = pNetwork.openStatus()) != Acad::eOk)
		return es;

	{
		AcDbAssocVariable* const pVar = new AcDbAssocVariable();

		if ((es = pNetwork->database()->addAcDbObject(varId, pVar)) != Acad::eOk)
			return es;

		pVar->close();

		if ((es = pNetwork->addAction(varId, true)) != Acad::eOk)
			return es;
	}

	AcDbObjectPointer<AcDbAssocVariable> pAssocVar(varId, kForWrite);

	AcString errMsgValidate(L"Name or Expression invalid");

	if ((es = pAssocVar->validateNameAndExpression(name, L"1.0", errMsgValidate)) != Acad::eOk)
		return es;

	if ((es = pAssocVar->setName(name, true)) != Acad::eOk)
		return es;

	pAssocVar->setValue(AcDbEvalVariant(1.0));

	if ((es = pAssocVar->setExpression(expression,
		EXPRESSION_EVALUATOR_DEFAULT,
		true,
		true)) != Acad::eOk)
		return es;

	AcString errMsgEval(L"Expression evaluation failed");

	AcDbEvalVariant evaluatedExpressionValue;

	if ((es = pAssocVar->evaluateExpression(evaluatedExpressionValue, errMsgEval)) != Acad::eOk)
		return es;

	if ((pAssocVar->setValue(evaluatedExpressionValue)) != Acad::eOk)
		return es;

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::addValueDependency(const AcDbObjectId& networkId,
	const AcDbObjectId& varId,
	AcDbObjectId& varDepId)
{
	Acad::ErrorStatus es = eOk;

	varDepId = AcDbObjectId::kNull;

	AcDbAssocValueDependency* pValDep = new AcDbAssocValueDependency();

	if ((es = networkId.database()->addAcDbObject(varDepId, pValDep)) != Acad::eOk)
	{
		delete pValDep;
		return es;
	}

	if ((es = pValDep->attachToObject(AcDbCompoundObjectId(varId))) != Acad::eOk)
		return es;

	if ((es = pValDep->close()) != Acad::eOk)
		return es;

	if (varId.isNull() || varDepId.isNull())
		return Acad::eNullObjectId;

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AcGeMatrix3d AdnAssocSampleUtils::getConstraintGroupCoordSysInv(AcDbAssoc2dConstraintGroup* p2dConstrGrp)
{
	AcGePlane plane = p2dConstrGrp->getWorkPlane();

	AcGePoint3d origin;
	AcGeVector3d xAxis, yAxis;
	plane.getCoordSystem(origin, xAxis, yAxis);

	AcGeMatrix3d coordSys;
	coordSys.setCoordSystem(origin, xAxis, yAxis, plane.normal());

	return coordSys.inverse();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::getClosestConstrainedPoint(AcConstrainedGeometry*& pConsGeom,
	const AcGePoint3d& entPtInConstGrp,
	AcConstrainedPoint*& point)
{
	if (!pConsGeom->isKindOf(AcConstrainedCurve::desc()))
		return eInvalidInput;

	AcConstrainedCurve* pConstrainedCurve = AcConstrainedCurve::cast(pConsGeom);

	AcArray<AcConstrainedImplicitPoint*> apImplicitPoints;

	pConstrainedCurve->getConstrainedImplicitPoints(apImplicitPoints);

	point = apImplicitPoints[0];

	double distMin = entPtInConstGrp.distanceTo(apImplicitPoints[0]->point());

	for (int i = 1; i < apImplicitPoints.length(); ++i)
	{
		double dist = entPtInConstGrp.distanceTo(apImplicitPoints[i]->point());

		if (dist < distMin)
		{
			distMin = dist;
			point = apImplicitPoints[i];
		}
	}

	return Acad::eOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AdnAssocSampleUtils::createDistanceConstraint(const AcDbObjectId& varDepId,
	const AcDbObjectId& dimDepId,
	AcConstrainedGeometry*& pConsGeom1,
	const AcGePoint3d& entPt1,
	AcConstrainedGeometry*& pConsGeom2,
	const AcGePoint3d& entPt2,
	const AcGeVector3d* pFixedDirection,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId,
	AcDistanceConstraint** ppNewDisConstraint)
{
	ErrorStatus es;

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(spaceId, true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);

	if ((es = p2dConstrGrp.openStatus()) != Acad::eOk)
		return es;

	AcGeMatrix3d coordSysInv(AdnAssocSampleUtils::getConstraintGroupCoordSysInv(p2dConstrGrp));

	//Get closest constrained point1
	AcGePoint3d entPt1InConstGrp = entPt1;
	entPt1InConstGrp.transformBy(coordSysInv);

	AcConstrainedPoint* point1;

	if ((es = AdnAssocSampleUtils::getClosestConstrainedPoint(pConsGeom1, entPt1InConstGrp, point1)) != Acad::eOk)
		return es;

	//Get closest constrained point2
	AcGePoint3d entPt2InConstGrp = entPt2;
	entPt2InConstGrp.transformBy(coordSysInv);

	AcConstrainedPoint* point2;

	if ((es = AdnAssocSampleUtils::getClosestConstrainedPoint(pConsGeom2, entPt2InConstGrp, point2)) != Acad::eOk)
		return es;

	if ((es = p2dConstrGrp->addDistanceConstraint(point1,
		point2,
		(pFixedDirection != NULL ? AcDistanceConstraint::kFixedDirection : AcDistanceConstraint::kNotDirected),
		varDepId,
		dimDepId,
		pFixedDirection,
		NULL,
		ppNewDisConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromDatabase(pDb, true, DICTIONARYKEY);
	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);
	AdnConstraintEvaluationCallback callback;
	pNetwork->evaluate(&callback);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::create2LineAngleConstraint(const AcDbObjectId& varDepId,
	const AcDbObjectId& dimDepId,
	AcConstrainedGeometry*& pConsGeom1,
	AcConstrainedGeometry*& pConsGeom2,
	AcAngleConstraint::SectorType sectorType,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId,
	AcAngleConstraint** ppNewAngConstraint)
{
	ErrorStatus es;

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(spaceId, true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);

	if ((es = p2dConstrGrp.openStatus()) != Acad::eOk)
		return es;

	if (!pConsGeom1->isKindOf(AcConstrainedLine::desc()))
		return eInvalidInput;

	if (!pConsGeom2->isKindOf(AcConstrainedLine::desc()))
		return eInvalidInput;

	AcConstrainedLine* pConsLine1 = AcConstrainedLine::cast(pConsGeom1);
	AcConstrainedLine* pConsLine2 = AcConstrainedLine::cast(pConsGeom2);

	if ((es = p2dConstrGrp->addAngleConstraint(pConsLine1,
		pConsLine2,
		sectorType,
		varDepId,
		dimDepId,
		ppNewAngConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromDatabase(pDb, true, DICTIONARYKEY);
	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);
	AdnConstraintEvaluationCallback callback;
	pNetwork->evaluate(&callback);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::create3PointAngleConstraint(const AcDbObjectId& varDepId,
	const AcDbObjectId& dimDepId,
	AcConstrainedGeometry*& pConsGeom1,
	const AcGePoint3d& entPt1,
	AcConstrainedGeometry*& pConsGeom2,
	const AcGePoint3d& entPt2,
	AcConstrainedGeometry*& pConsGeom3,
	const AcGePoint3d& entPt3,
	AcAngleConstraint::SectorType sectorType,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId,
	Ac3PointAngleConstraint** ppNewAngConstraint)
{
	ErrorStatus es;

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(spaceId, true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);

	if ((es = p2dConstrGrp.openStatus()) != Acad::eOk)
		return es;

	AcGeMatrix3d coordSysInv(AdnAssocSampleUtils::getConstraintGroupCoordSysInv(p2dConstrGrp));

	//Get closest constrained point1
	AcGePoint3d entPt1InConstGrp = entPt1;
	entPt1InConstGrp.transformBy(coordSysInv);

	AcConstrainedPoint* point1;

	if ((es = getClosestConstrainedPoint(pConsGeom1, entPt1InConstGrp, point1)) != Acad::eOk)
		return es;

	//Get closest constrained point2
	AcGePoint3d entPt2InConstGrp = entPt2;
	entPt2InConstGrp.transformBy(coordSysInv);

	AcConstrainedPoint* point2;

	if ((es = getClosestConstrainedPoint(pConsGeom2, entPt2InConstGrp, point2)) != Acad::eOk)
		return es;

	//Get closest constrained point3
	AcGePoint3d entPt3InConstGrp = entPt3;
	entPt3InConstGrp.transformBy(coordSysInv);

	AcConstrainedPoint* point3;

	if ((es = getClosestConstrainedPoint(pConsGeom3, entPt3InConstGrp, point3)) != Acad::eOk)
		return es;

	if ((es = p2dConstrGrp->add3PointAngleConstraint(point1,
		point2,
		point3,
		sectorType,
		varDepId,
		dimDepId,
		ppNewAngConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromDatabase(pDb, true, DICTIONARYKEY);
	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);
	AdnConstraintEvaluationCallback callback;
	pNetwork->evaluate(&callback);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus
AdnAssocSampleUtils::createRadiusDiameterConstraint(const AcDbObjectId& varDepId,
	const AcDbObjectId& dimDepId,
	AcConstrainedGeometry*& pConsGeom,
	AcRadiusDiameterConstraint::RadiusDiameterConstrType constrType,
	AcDbDatabase* pDb,
	const AcDbObjectId& spaceId,
	AcRadiusDiameterConstraint** ppNewRadDiaConstraint)
{
	ErrorStatus es;

	AcDbObjectId consGrpId = AdnAssocSampleUtils::getConstraintGroup(spaceId, true);

	if (consGrpId.isNull())
		return Acad::eNullObjectId;

	AcDbObjectPointer<AcDbAssoc2dConstraintGroup> p2dConstrGrp(consGrpId, kForWrite);

	if ((es = p2dConstrGrp.openStatus()) != Acad::eOk)
		return es;

	if (!pConsGeom->isKindOf(AcConstrainedCircle::desc()))
		return eInvalidInput;

	if ((es = p2dConstrGrp->addRadiusDiameterConstraint(pConsGeom, constrType, varDepId, dimDepId, ppNewRadDiaConstraint)) != Acad::eOk)
		return es;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromDatabase(pDb, true, DICTIONARYKEY);
	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForWrite);
	AdnConstraintEvaluationCallback callback;
	pNetwork->evaluate(&callback);

	return es;
}

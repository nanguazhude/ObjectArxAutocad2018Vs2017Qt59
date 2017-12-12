
#include "ADNAssocCreateConstraint.hpp"
#include "AdnAssocSampleUtils.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createCoincidentConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1, 
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(
		entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(
		entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 


	AcDbFullSubentPathArray aVertexPaths;

	AcDbFullSubentPath vertEntPath1;
	AcGePoint3d vertexPos1;
	if((es = AdnAssocSampleUtils::getClosestVertexInfo(
		entId1, edgeEntPath1, ptEnt1, vertexPos1, vertEntPath1)) != Acad::eOk)
		return es;
	
	aVertexPaths.append(vertEntPath1);
	
	AcDbFullSubentPath vertEntPath2;
	AcGePoint3d vertexPos2;
	if((es = AdnAssocSampleUtils::getClosestVertexInfo(
		entId2, edgeEntPath2, ptEnt2, vertexPos2, vertEntPath2)) != Acad::eOk)
		return es;

	aVertexPaths.append(vertEntPath2);
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kCoincident, 
		aVertexPaths,
		fixedGeom);
	
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createParallelConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kParallel, 
		aEdgePaths,
		fixedGeom);
	
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createPerpendicularConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kPerpendicular, 
		aEdgePaths,
		fixedGeom);
	
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createConcentricConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kConcentric);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kConcentric);
		
	if(!(isValidCrv1 && isValidCrv2))
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kConcentric, 
		aEdgePaths,
		fixedGeom);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createColinearConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kColinear);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kColinear);
		
	if(!(isValidCrv1 && isValidCrv2))
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kColinear, 
		aEdgePaths,
		fixedGeom);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createHorizontalConstraint(
	AcDbObjectId& entId, 
	AcGePoint3d& ptEnt)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId, ptEnt, AcGeomConstraint::kHorizontal);

	if(!isValidCrv1)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, ptEnt, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	es = AdnAssocSampleUtils::addGeomConstraint(AcGeomConstraint::kHorizontal, aEdgePaths);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createVerticalConstraint(
	AcDbObjectId& entId,
	AcGePoint3d& ptEnt)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId, ptEnt, AcGeomConstraint::kVertical);

	if(!isValidCrv1)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, ptEnt, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	es = AdnAssocSampleUtils::addGeomConstraint(AcGeomConstraint::kVertical, aEdgePaths);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createFixConstraint(
	AcDbObjectId& entId,
	AcGePoint3d& ptEnt)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId, ptEnt, AcGeomConstraint::kFix);

	if(!isValidCrv1)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, ptEnt, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	AcDbFullSubentPathArray aVertexPaths;

	AcDbFullSubentPath vertEntPath1;
	AcGePoint3d vertexPos1;
	if((es = AdnAssocSampleUtils::getClosestVertexInfo(entId, edgeEntPath1, ptEnt, vertexPos1, vertEntPath1)) != Acad::eOk)
		return es;
	
	aVertexPaths.append(vertEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(AcGeomConstraint::kFix, aVertexPaths);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createTangentConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kTangent);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kTangent);
		
	if(!isValidCrv1 || !isValidCrv2)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kTangent, 
		aEdgePaths,
		fixedGeom);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createSmoothConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kG2Smooth);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kG2Smooth);

	if(!isValidCrv1 || !isValidCrv2)
		return Acad::eInvalidInput;
	
	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	AcGePoint3d pos1;

	if((es = AdnAssocSampleUtils::getClosestVertexPos(entId1, edgeEntPath1, ptEnt1, pos1)) != Acad::eOk)
		return es;

	AcGePoint3d pos2;

	if((es = AdnAssocSampleUtils::getClosestVertexPos(entId2, edgeEntPath2, ptEnt2, pos2)) != Acad::eOk)
		return es;

	AcGePoint3dArray points;

	points.append(pos1);
	points.append(pos2);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kG2Smooth, 
		pConsGeoms, 
		points);
	
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createSymmetricConstraint(
	AcDbObjectId& entId1, // ObjectID of first entity
	AcDbObjectId& entId2, // ObjectID of second entity 
	AcDbObjectId& entSymId, // ObjectID of third entity 
	AcGePoint3d& ptEnt1,  // Point on first entity used to determine which edge subentity is closest to it
	AcGePoint3d& ptEnt2,  // Point on second entity used to determine which edge subentity is closest to it
	AcGePoint3d& ptEntSym)  // Point on third entity used to determine which edge subentity is closest to it											
{
	Acad::ErrorStatus es = Acad::eOk;
	
	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kSymmetric);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kSymmetric);
	bool isValidCrv3 = AdnAssocSampleUtils::isEntityValidForConstraint(entSymId, ptEntSym, AcGeomConstraint::kSymmetric);

	if(!isValidCrv1 || !isValidCrv2 || !isValidCrv3)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 

	AcDbFullSubentPath edgeEntPath3;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entSymId, ptEntSym, edgeEntPath3)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath3);
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);
	fixedGeom.append(edgeEntPath3);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kSymmetric, 
		aEdgePaths,
		fixedGeom);
	
	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createEqualLengthConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,  
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kEqualLength);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kEqualLength);
		
	if(!isValidCrv1 || !isValidCrv2)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kEqualLength, 
		aEdgePaths,
		fixedGeom);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createEqualRadiusConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& ptEnt1,
	AcGePoint3d& ptEnt2)
{
	Acad::ErrorStatus es = Acad::eOk;

	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	bool isValidCrv1 = AdnAssocSampleUtils::isEntityValidForConstraint(entId1, ptEnt1, AcGeomConstraint::kEqualRadius);
	bool isValidCrv2 = AdnAssocSampleUtils::isEntityValidForConstraint(entId2, ptEnt2, AcGeomConstraint::kEqualRadius);
		
	if(!isValidCrv1 || !isValidCrv2)
		return Acad::eInvalidInput;

	AcDbFullSubentPathArray aEdgePaths;

	AcDbFullSubentPath edgeEntPath1;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, ptEnt1, edgeEntPath1)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath1); 

	AcDbFullSubentPath edgeEntPath2;
	if((es = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, ptEnt2, edgeEntPath2)) != Acad::eOk)
		return es;
	
	aEdgePaths.append(edgeEntPath2); 
	
	AcArray<AcConstrainedGeometry*> pConsGeoms;
	if((es = AdnAssocSampleUtils::addConstrainedGeometry(aEdgePaths, pConsGeoms)) != Acad::eOk)
		return es;

	// hint fixed geometry
	AcDbFullSubentPathArray fixedGeom;

	fixedGeom.append(edgeEntPath1);

	es = AdnAssocSampleUtils::addGeomConstraint(
		AcGeomConstraint::kEqualRadius, 
		aEdgePaths,
		fixedGeom);

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
	AcDbObjectId& entId1, 
    AcGePoint3d& entPt1,  
    AcDbObjectId& entId2, 
    AcGePoint3d& entPt2,  
    AcGePoint3d& dimPos,  
	AcDbObjectId& varDimId)
{
	AcDbObjectIdArray cleanupObjectIds;

	varDimId = AcDbObjectId::kNull;
	AcDbObjectId dimId = AcDbObjectId::kNull;
	AcDbObjectId varDepId = AcDbObjectId::kNull;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId spaceId = pDb->currentSpaceId();
	AcDbObjectId dimStyleId = AcDbObjectId::kNull;

	AcDistanceConstraint** ppNewDisConstraint = NULL;

	try
	{
		Acad::ErrorStatus err = Acad::eOk;

		if((err = AcDbAssocManager::initialize()) != Acad::eOk)
			throw err;

		AcDbFullSubentPathArray aPathsEdge;

		// Take the first edge
		AcDbFullSubentPath edgeEntPath1;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, entPt1, edgeEntPath1)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath1); 

		AcGePoint3d vertex1;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId1, edgeEntPath1, entPt1, vertex1)) != Acad::eOk)
			throw err;
															  
		// Take the second edge
		AcDbFullSubentPath edgeEntPath2;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, entPt2, edgeEntPath2)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath2); 

		AcGePoint3d vertex2;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId2, edgeEntPath2, entPt2, vertex2)) != Acad::eOk)
			throw err;

		AcArray<AcConstrainedGeometry*> pConsGeoms;
		if((err = AdnAssocSampleUtils::addConstrainedGeometry(aPathsEdge, pConsGeoms)) != Acad::eOk)
			throw err;

		if((err = AdnAssocSampleUtils::createAlignedDim(
			vertex1, 
			vertex2, 
			dimPos, 
			dimId,
			dimStyleId,
			pDb,
			spaceId)) != Acad::eOk)
			throw err;

		cleanupObjectIds.append(dimId);
								
		AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

		AcString dimName;
		AcString dimExpression;

		if((err = AdnAssocSampleUtils::getNameAndExpressionFromDimension(
			networkId, 
			dimId, 
			AcDbAssocConstraintType::kDistanceAssocConstraintType, 
			dimName, 
			dimExpression)) != Acad::eOk)
				throw err;

		//Create new AssocVariable
		if((err = AdnAssocSampleUtils::addNewVariableToAssocNetwork(
			networkId, 
			dimName, 
			dimExpression, 
			varDimId))!= Acad::eOk)
			throw err;

		//As the variable didn't exist, we will need to clean it up if something goes wrong from here.
		cleanupObjectIds.append(varDimId);

		if((err = AdnAssocSampleUtils::addValueDependency(networkId, varDimId, varDepId)) != Acad::eOk)
			throw err;

		//create dim dependency
		AcDbObjectId dimDepBodyId, dimDepId;

		if((err = AcDbAssocDimDependencyBody::createAndPostToDatabase(dimId, dimDepId, dimDepBodyId)) != Acad::eOk)
			throw err;

		const bool bPreviousValue = AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(false);

		if((err = AdnAssocSampleUtils::createDistanceConstraint(
			varDepId, 
			dimDepId, 
			pConsGeoms[0],
			vertex1,
			pConsGeoms[1],
			vertex2,
			NULL,
			pDb,
			spaceId,
			ppNewDisConstraint))!= Acad::eOk)
			throw err;

		AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bPreviousValue);

		return err;
	}
	catch(Acad::ErrorStatus err)
	{
		if(err != Acad::eOk)
		{
			AdnAssocSampleUtils::doCleanUp(cleanupObjectIds);
		}
		return err;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
	AcDbObjectId& entId, 
	AcGePoint3d& entPt1, 
	AcGePoint3d& entPt2, 
	AcGePoint3d& dimPos, 
	AcDbObjectId& varDimId)
{
	return AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
		entId, entPt1, entId, entPt2, dimPos, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(
	AcDbObjectId& entId,
	AcGePoint3d& dimPos, 
	AcDbSubentId subentId,
	AcDbObjectId& varDimId)
{
	Acad::ErrorStatus err = Acad::eOk;

	AcDbSmartObjectPointer <AcDbEntity> pEntity(entId, AcDb::kForRead);

	if((err = pEntity.openStatus()) != Acad::eOk )
		return err;

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE = 
		AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcGeCurve3d* segment = NULL;
	if((err = pAssocPersSubentIdPE->getEdgeSubentityGeometry(pEntity, subentId, segment))!= Acad::eOk )
		return err;

	AcGePoint3d startPoint;
	if(!segment->hasStartPoint(startPoint))
	{
		delete segment;
		return Acad::eInvalidInput;
	}

	AcGePoint3d endPoint;
	if(!segment->hasEndPoint(endPoint))
	{
		delete segment;
		return Acad::eInvalidInput;
	}

	delete segment;

	return AcDbAssoc2dConstraintAPI::createAlignedDimConstraint(entId, startPoint, endPoint, dimPos, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::create2LineAngularDimConstraint(
	AcDbObjectId& entId1, 
    AcDbObjectId& entId2,
	AcGePoint3d& entPt1, 
    AcGePoint3d& entPt2,  
    AcGePoint3d& dimPos, 
	AcDbObjectId& varDimId)
{
	AcDbObjectIdArray cleanupObjectIds;

	varDimId = AcDbObjectId::kNull;
	AcDbObjectId dimId = AcDbObjectId::kNull;
	AcDbObjectId varDepId = AcDbObjectId::kNull;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId spaceId = pDb->currentSpaceId();
	AcDbObjectId dimStyleId = AcDbObjectId::kNull;

	AcAngleConstraint** ppNewAngConstraint = NULL;

	try
	{
		Acad::ErrorStatus err = Acad::eOk;

		if((err = AcDbAssocManager::initialize()) != Acad::eOk)
			throw err;

		AcDbFullSubentPath edgeEntPath1;
		AcGePoint3d startPoint1;
		AcGePoint3d endPoint1;
		
		if((err = AdnAssocSampleUtils::getLinearEdgeData(entId1, entPt1, edgeEntPath1, startPoint1, endPoint1)) != Acad::eOk)
			throw err;

		AcDbFullSubentPath edgeEntPath2;
		AcGePoint3d startPoint2;
		AcGePoint3d endPoint2;
		
		if((err = AdnAssocSampleUtils::getLinearEdgeData(entId2, entPt2, edgeEntPath2, startPoint2, endPoint2)) != Acad::eOk)
			throw err;
														    
		if((err = AdnAssocSampleUtils::create2LinesAngularDim(startPoint2, 
															  endPoint2, 
															  startPoint1, 
															  endPoint1, 
															  dimPos, 
															  dimId,
															  dimStyleId,
															  pDb,
															  spaceId)) != Acad::eOk)
			throw err;

		cleanupObjectIds.append(dimId);

		AcDbFullSubentPathArray aPathsEdge;

		aPathsEdge.append(edgeEntPath1);
		aPathsEdge.append(edgeEntPath2);

		AcArray<AcConstrainedGeometry*> pConsGeoms;
		if((err = AdnAssocSampleUtils::addConstrainedGeometry(aPathsEdge, pConsGeoms)) != Acad::eOk)
			throw err;

		AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

		AcString dimName;
		AcString dimExpression;

		if((err = AdnAssocSampleUtils::getNameAndExpressionFromDimension(networkId, 
																		 dimId, 
																		 AcDbAssocConstraintType::kAngle0AssocConstraintType, 
																		 dimName, 
																		 dimExpression)) != Acad::eOk)
				throw err;

		//Create new AssocVariable
		if((err = AdnAssocSampleUtils::addNewVariableToAssocNetwork(networkId, dimName, dimExpression, varDimId))!= Acad::eOk)
			throw err;

		//As the variable didn't exist, we will need to clean it up if something goes wrong from here.
		cleanupObjectIds.append(varDimId);

		if((err = AdnAssocSampleUtils::addValueDependency(networkId, varDimId, varDepId)) != Acad::eOk)
			throw err;

		//create dim dependency
		AcDbObjectId dimDepBodyId, dimDepId;

		if((err = AcDbAssocDimDependencyBody::createAndPostToDatabase(dimId, dimDepId, dimDepBodyId)) != Acad::eOk)
			throw err;

		const bool bPreviousValue = AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(false);

		AcAngleConstraint::SectorType sectorType = AcAngleConstraint::SectorType::kParallelClockwise;
		
		if((err = AdnAssocSampleUtils::create2LineAngleConstraint(varDepId, 
																  dimDepId, 
																  pConsGeoms[0],
																  pConsGeoms[1],
																  sectorType,
																  pDb,
																  spaceId,
																  ppNewAngConstraint))!= Acad::eOk)
			throw err;

		AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bPreviousValue);

		return err;
	}
	catch(Acad::ErrorStatus err)
	{
		if(err != Acad::eOk)
		{
			AdnAssocSampleUtils::doCleanUp(cleanupObjectIds);
		}
		return err;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createArcAngularDimConstraint(
	AcDbObjectId& entId, 
	AcGePoint3d& entPt,  
	AcGePoint3d& dimPos, 
	AcDbObjectId& varDimId)
{
	Acad::ErrorStatus err = Acad::eOk;

	if((err = AcDbAssocManager::initialize()) != Acad::eOk)
		return err;

	AcDbFullSubentPathArray aPathsEdge;

	AcDbFullSubentPath edgeEntPath;
	if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, entPt, edgeEntPath)) != Acad::eOk)
		return err;

	aPathsEdge.append(edgeEntPath); 

	AcDbSmartObjectPointer <AcDbEntity> pEntity(entId, AcDb::kForRead);

	if((err = pEntity.openStatus()) != Acad::eOk )
		return err;

	AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE = AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

	AcGeCurve3d* segment = NULL;
	if((err = pAssocPersSubentIdPE->getEdgeSubentityGeometry(pEntity, edgeEntPath.subentId(), segment))!= Acad::eOk )
		return err;

	if(!(segment->type() == AcGe::EntityId::kCircArc3d))
	{
		delete segment;
		return Acad::eInvalidInput;
	}

	AcGeCircArc3d* arc = (AcGeCircArc3d*)segment;

	AcDbObjectId dimId;

	AcGePoint3d centerPoint = arc->center();
	AcGePoint3d xLine1Point = arc->startPoint();	
	AcGePoint3d xLine2Point = arc->endPoint();

	delete arc;

	return AcDbAssoc2dConstraintAPI::create3PointAngularDimConstraint(
		entId, 
		entId,
		entId,
		centerPoint, 
		xLine1Point, 
		xLine2Point, 
		dimPos, 
		varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::create3PointAngularDimConstraint(
	AcDbObjectId& entId1,  
	AcDbObjectId& entId2,
	AcDbObjectId& entId3, 
	AcGePoint3d& entPt1, 
	AcGePoint3d& entPt2,   
	AcGePoint3d& entPt3,   
	AcGePoint3d& dimPos,   
	AcDbObjectId& varDimId)
{
	AcDbObjectIdArray cleanupObjectIds;

	varDimId = AcDbObjectId::kNull;
	AcDbObjectId dimId = AcDbObjectId::kNull;
	AcDbObjectId varDepId = AcDbObjectId::kNull;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId spaceId = pDb->currentSpaceId();
	AcDbObjectId dimStyleId = AcDbObjectId::kNull;

	Ac3PointAngleConstraint** ppNewAngConstraint = NULL;
		
	try
	{
		Acad::ErrorStatus err = Acad::eOk;

		if((err = AcDbAssocManager::initialize()) != Acad::eOk)
			throw err;

		AcDbFullSubentPathArray aPathsEdge;

		//Get Edge/Vertex 1
		AcDbFullSubentPath edgeEntPath1;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, entPt1, edgeEntPath1)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath1); 

		AcGePoint3d vertex1;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId1, edgeEntPath1, entPt1, vertex1)) != Acad::eOk)
			throw err;

		//Get Edge/Vertex 2
		AcDbFullSubentPath edgeEntPath2;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, entPt2, edgeEntPath2)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath2); 

		AcGePoint3d vertex2;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId2, edgeEntPath2, entPt2, vertex2)) != Acad::eOk)
			throw err;

		//Get Edge/Vertex 3
		AcDbFullSubentPath edgeEntPath3;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId3, entPt3, edgeEntPath3)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath3); 

		AcGePoint3d vertex3;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId3, edgeEntPath3, entPt3, vertex3)) != Acad::eOk)
			throw err;

		AcArray<AcConstrainedGeometry*> pConsGeoms;

		if((err = AdnAssocSampleUtils::addConstrainedGeometry(aPathsEdge, pConsGeoms)) != Acad::eOk)
			throw err;
														    
		if((err = AdnAssocSampleUtils::create3PointAngularDim(vertex1, 
			                                                  vertex3, 
															  vertex2, 
															  dimPos, 
															  dimId,
															  dimStyleId,
															  pDb,
															  spaceId)) != Acad::eOk)
			throw err;

		cleanupObjectIds.append(dimId);

		AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

		AcString dimName;
		AcString dimExpression;

		if((err = AdnAssocSampleUtils::getNameAndExpressionFromDimension(networkId, 
																		 dimId, 
																		 AcDbAssocConstraintType::kAngle0AssocConstraintType, 
																		 dimName, 
																		 dimExpression)) != Acad::eOk)
				throw err;

		//Create new AssocVariable
		if((err = AdnAssocSampleUtils::addNewVariableToAssocNetwork(networkId, dimName, dimExpression, varDimId))!= Acad::eOk)
			throw err;

		//As the variable didn't exist, we will need to clean it up if something goes wrong from here.
		cleanupObjectIds.append(varDimId);

		if((err = AdnAssocSampleUtils::addValueDependency(networkId, varDimId, varDepId)) != Acad::eOk)
			throw err;

		//create dim dependency
		AcDbObjectId dimDepBodyId, dimDepId;

		if((err = AcDbAssocDimDependencyBody::createAndPostToDatabase(dimId, dimDepId, dimDepBodyId)) != Acad::eOk)
			throw err;

		const bool bPreviousValue = AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(false);

		AcAngleConstraint::SectorType sectorType = AcAngleConstraint::SectorType::kParallelClockwise;

		if((err = AdnAssocSampleUtils::create3PointAngleConstraint(varDepId, 
																   dimDepId, 
																   pConsGeoms[0],
																   vertex1,
																   pConsGeoms[1],
																   vertex2,
																   pConsGeoms[2],
																   vertex3,
																   sectorType,
																   pDb,
																   spaceId,
																   ppNewAngConstraint))!= Acad::eOk)
			throw err;

		AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bPreviousValue);

		return err;
	}
	catch(Acad::ErrorStatus err)
	{
		if(err != Acad::eOk)
		{
			AdnAssocSampleUtils::doCleanUp(cleanupObjectIds);
		}
		return err;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus createRadialOrDiamDimConstraint(
	AcDbObjectId& entId, 
	AcGePoint3d& entPt,  
	AcGePoint3d& dimPos, 
	bool isRadial,
	AcDbObjectId& varDimId )
{
	AcDbObjectIdArray cleanupObjectIds;

	varDimId = AcDbObjectId::kNull;
	AcDbObjectId dimId = AcDbObjectId::kNull;
	AcDbObjectId varDepId = AcDbObjectId::kNull;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId spaceId = pDb->currentSpaceId();
	AcDbObjectId dimStyleId = AcDbObjectId::kNull;

	AcRadiusDiameterConstraint** ppNewRadDiaConstraint = NULL;

	try
	{
		Acad::ErrorStatus err = Acad::eOk;

		if((err = AcDbAssocManager::initialize()) != Acad::eOk)
			throw err;

		AcDbFullSubentPathArray aPathsEdge;

		AcDbFullSubentPath edgeEntPath;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId, entPt, edgeEntPath)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath); 

		AcDbSmartObjectPointer <AcDbEntity> pEntity(entId, AcDb::kForRead);

		if((err = pEntity.openStatus()) != Acad::eOk )
			throw err;

		AcDbAssocPersSubentIdPE* const pAssocPersSubentIdPE = AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));

		AcGeCurve3d* segment = NULL;
		if((err = pAssocPersSubentIdPE->getEdgeSubentityGeometry(pEntity, edgeEntPath.subentId(), segment))!= Acad::eOk )
			throw err;

		if(!(segment->type() == AcGe::EntityId::kCircArc3d))
		{
			delete segment;
			throw Acad::eInvalidInput;
		}

		AcGeCircArc3d* arc = (AcGeCircArc3d*)segment;

		AcGePoint3d center = arc->center();

		AcGeVector3d dir(dimPos-center);

		if(dir.length() > 0.001)
			dir	= dir * (arc->radius() / dir.length());

		AcGePoint3d chordPt((center.x + dir.x),
							(center.y + dir.y),
							(center.z + dir.z));
		
		double leaderLength = chordPt.distanceTo(dimPos);

		AcDbAssocConstraintType constrTypeForName;
		AcRadiusDiameterConstraint::RadiusDiameterConstrType constrType;

		if(isRadial)
		{
			err = AdnAssocSampleUtils::createRadialDim(entId, center, chordPt, leaderLength, dimId, dimStyleId, pDb, spaceId);

			constrTypeForName = AcDbAssocConstraintType::kRadiusAssocConstraintType;
			constrType = AcRadiusDiameterConstraint::RadiusDiameterConstrType::kCircleRadius;
		}
		else
		{
			AcGePoint3d farChordPt((center.x - dir.x),
								   (center.y - dir.y),
								   (center.z - dir.z));

			err = AdnAssocSampleUtils::createDiametricDim(entId, chordPt, farChordPt, dimPos, leaderLength, dimId, dimStyleId, pDb, spaceId);

			constrTypeForName = AcDbAssocConstraintType::kDiameterAssocConstraintType;
			constrType = AcRadiusDiameterConstraint::RadiusDiameterConstrType::kCircleDiameter;
		}

		delete arc;

		if(err != Acad::eOk)
			throw err;

		cleanupObjectIds.append(dimId);

		AcArray<AcConstrainedGeometry*> pConsGeoms;
		if((err = AdnAssocSampleUtils::addConstrainedGeometry(aPathsEdge, pConsGeoms)) != Acad::eOk)
			throw err;

		AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

		AcString dimName;
		AcString dimExpression;

		if((err = AdnAssocSampleUtils::getNameAndExpressionFromDimension(networkId, 
																		 dimId, 
																		 constrTypeForName, 
																		 dimName, 
																		 dimExpression)) != Acad::eOk)
				throw err;

		//Create new AssocVariable
		if((err = AdnAssocSampleUtils::addNewVariableToAssocNetwork(networkId, dimName, dimExpression, varDimId))!= Acad::eOk)
			throw err;

		//As the variable didn't exist, we will need to clean it up if something goes wrong from here.
		cleanupObjectIds.append(varDimId);

		if((err = AdnAssocSampleUtils::addValueDependency(networkId, varDimId, varDepId)) != Acad::eOk)
			throw err;

		//create dim dependency
		AcDbObjectId dimDepBodyId, dimDepId;

		if((err = AcDbAssocDimDependencyBody::createAndPostToDatabase(dimId, dimDepId, dimDepBodyId)) != Acad::eOk)
			throw err;

		const bool bPreviousValue = AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(false);

		if((err = AdnAssocSampleUtils::createRadiusDiameterConstraint(varDepId, 
																	  dimDepId, 
																	  pConsGeoms[0],
																	  constrType, 
																	  pDb,
																	  spaceId,
																	  ppNewRadDiaConstraint))!= Acad::eOk)
			throw err;

		AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bPreviousValue);

		return err;
	}
	catch(Acad::ErrorStatus err)
	{
		if(err != Acad::eOk)
		{
			AdnAssocSampleUtils::doCleanUp(cleanupObjectIds);
		}
		return err;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createRadialDimConstraint(
	AcDbObjectId& entId, 
	AcGePoint3d& entPt,  
	AcGePoint3d& dimPos, 
	AcDbObjectId& varDimId)
{
	bool isRadial = true;

	return createRadialOrDiamDimConstraint(entId, entPt, dimPos, isRadial, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createDiamDimConstraint(
	AcDbObjectId& entId,  
	AcGePoint3d& entPt,   
	AcGePoint3d& dimPos,  
	AcDbObjectId& varDimId)
{
	bool isRadial = false;

	return createRadialOrDiamDimConstraint(entId, entPt, dimPos, isRadial, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus createRotatedDimConstraint(
	AcDbObjectId& entId1, 
    AcGePoint3d& entPt1, 
	AcDbObjectId& entId2, 
    AcGePoint3d& entPt2, 
    AcGePoint3d& dimPos,
	double rotation,
	const AcGeVector3d& fixedDirection,
	AcDbObjectId& varDimId)
{
	AcDbObjectIdArray cleanupObjectIds;

	varDimId = AcDbObjectId::kNull;
	AcDbObjectId dimId = AcDbObjectId::kNull;
	AcDbObjectId varDepId = AcDbObjectId::kNull;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbObjectId spaceId = pDb->currentSpaceId();
	AcDbObjectId dimStyleId = AcDbObjectId::kNull;

	AcDistanceConstraint** ppNewDisConstraint = NULL;

	try
	{
		Acad::ErrorStatus err = Acad::eOk;

		if((err = AcDbAssocManager::initialize()) != Acad::eOk)
			throw err;

		AcDbFullSubentPathArray aPathsEdge;

		// Take the first edge
		AcDbFullSubentPath edgeEntPath1;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId1, entPt1, edgeEntPath1)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath1); 

		AcGePoint3d vertex1;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId1, edgeEntPath1, entPt1, vertex1)) != Acad::eOk)
			throw err;
															  
		// Take the second edge
		AcDbFullSubentPath edgeEntPath2;
		if((err = AdnAssocSampleUtils::getClosestEdgeSubEntPath(entId2, entPt2, edgeEntPath2)) != Acad::eOk)
			throw err;

		aPathsEdge.append(edgeEntPath2); 

		AcGePoint3d vertex2;
		if((err = AdnAssocSampleUtils::getClosestVertexPos(entId2, edgeEntPath2, entPt2, vertex2)) != Acad::eOk)
			throw err;

		AcArray<AcConstrainedGeometry*> pConsGeoms;

		if((err = AdnAssocSampleUtils::addConstrainedGeometry(aPathsEdge, pConsGeoms)) != Acad::eOk)
			throw err;
		
		if((err = AdnAssocSampleUtils::createRotatedDim(entId1, 
														vertex1, 
														edgeEntPath1, 
														entId2, 
														vertex2, 
														edgeEntPath2, 
														dimPos, 
														rotation, 
														dimId,
														dimStyleId,
														pDb,
														spaceId)) != Acad::eOk)
			throw err;
						
		cleanupObjectIds.append(dimId);

		AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(spaceId, true);

		AcString dimName;
		AcString dimExpression;

		if((err = AdnAssocSampleUtils::getNameAndExpressionFromDimension(networkId, 
																		 dimId, 
																		 AcDbAssocConstraintType::kDistanceAssocConstraintType, 
																		 dimName, 
																		 dimExpression)) != Acad::eOk)
				throw err;

		//Create new AssocVariable
		if((err = AdnAssocSampleUtils::addNewVariableToAssocNetwork(networkId, dimName, dimExpression, varDimId))!= Acad::eOk)
			throw err;

		//As the variable didn't exist, we will need to clean it up if something goes wrong from here.
		cleanupObjectIds.append(varDimId);

		if((err = AdnAssocSampleUtils::addValueDependency(networkId, varDimId, varDepId)) != Acad::eOk)
			throw err;

		//create dim dependency
		AcDbObjectId dimDepBodyId, dimDepId;

		if((err = AcDbAssocDimDependencyBody::createAndPostToDatabase(dimId, dimDepId, dimDepBodyId)) != Acad::eOk)
			throw err;

		const bool bPreviousValue = AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(false);

		AcGeMatrix3d ucs;
		acedGetCurrentUCS(ucs);
		AcGeVector3d fixedDirectionUcs(fixedDirection);
		fixedDirectionUcs.transformBy(ucs);

		if((err = AdnAssocSampleUtils::createDistanceConstraint(varDepId, 
																dimDepId, 
																pConsGeoms[0],
																vertex1,
																pConsGeoms[1],
																vertex2,
																&fixedDirectionUcs,
																pDb,
																spaceId,
																ppNewDisConstraint))!= Acad::eOk)
			throw err;

		AcDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bPreviousValue);

		return err;
	}
	catch(Acad::ErrorStatus err)
	{
		if(err != Acad::eOk)
		{
			AdnAssocSampleUtils::doCleanUp(cleanupObjectIds);
		}
		return err;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createHorizontalDimConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& entPt1,
    AcGePoint3d& entPt2,  
    AcGePoint3d& dimPos,  
	AcDbObjectId& varDimId)
{
	double rotation = 0.0;

	AcGeVector3d fixedDirection(1, 0, 0);

	return createRotatedDimConstraint(entId1, entPt1, entId2, entPt2, dimPos, rotation, fixedDirection, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::createVerticalDimConstraint(
	AcDbObjectId& entId1, 
	AcDbObjectId& entId2, 
	AcGePoint3d& entPt1,
	AcGePoint3d& entPt2,  
	AcGePoint3d& dimPos,  
	AcDbObjectId& varDimId)
{
	double rotation = 2.0 * atan(1.0); // 2.0 * atan(1.0) = Pi/2

	AcGeVector3d fixedDirection(0, 1, 0);

	return createRotatedDimConstraint(entId1, entPt1, entId2, entPt2, dimPos, rotation, fixedDirection, varDimId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::getVariableByName(
	const AcDbObjectId& btrId, 
	const AcString& name,
	bool createIfDoesNotExist,
	AcDbObjectId& variableId)
{
	Acad::ErrorStatus es;

	if(name.length() == 0)
		return Acad::eInvalidInput;

	// Initialize Associative Framework
	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbObjectId networkId = AcDbAssocNetwork::getInstanceFromObject(btrId, true);

	if (networkId.isNull()) 
	{
		return Acad::eInvalidInput;
	}

	AcDbObjectPointer<AcDbAssocNetwork> pNetwork(networkId, kForRead);
		
	if ((es = pNetwork.openStatus()) != Acad::eOk)
		return es;
	
	//Iterate the network, trying to find an existing AssocVariable with the same name
	AcDbAssocNetworkIterator iter(pNetwork);
	
	while (iter.moveNext())
	{
		if (iter.current().objectClass() != AcDbAssocVariable::desc())
			continue;

		AcDbObjectPointer<AcDbAssocVariable> pAssocVar(iter.current(), kForRead);

		const AcString varName = pAssocVar->name();

		//Check if we found our guy
		if (varName.match(name) == name.length())
		{
			variableId = iter.current();
			return Acad::eOk;
		}
	}

	//If we don't want to create a new variable, returns an error
	if(!createIfDoesNotExist)
	{
		return Acad::eNullObjectId;
	}

	//Need to close network explicitely, otherwise addNewVariableToAssocNetwork will fail
	pNetwork->close();

	//Need to provide a default initial expression
	AcString expression(L"1.0");
	
	return AdnAssocSampleUtils::addNewVariableToAssocNetwork(networkId, 
														     name, 
														     expression,
														     variableId);												 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::renameVariable(
	const AcDbObjectId& variableId,
	const AcString& newName,
	bool updateReferencingExpressions)
{
	Acad::ErrorStatus es;

	// Initialize Associative Framework
	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbObjectPointer<AcDbAssocVariable> pAssocVar(variableId, kForRead);

	if ((es = pAssocVar.openStatus()) != Acad::eOk)
		return es;

	AcString errMsgValidate(L"Name or Expression invalid");

	if((es = pAssocVar->validateNameAndExpression(newName, pAssocVar->expression(), errMsgValidate)) != Acad::eOk)
		return es;

	if ((es = pAssocVar->upgradeOpen()) != Acad::eOk)
		return es;

	if ((es = pAssocVar->setName(newName, updateReferencingExpressions)) != Acad::eOk)
		return es;

	return es;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::getVariableValue(
	const AcDbObjectId& variableId,
	AcString&           name,
	AcDbEvalVariant&    value,
	AcString&           expression,
	AcString&           evaluatorId)
{
	Acad::ErrorStatus es;

	// Initialize Associative Framework
	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbObjectPointer<AcDbAssocVariable> pAssocVar(variableId, kForRead);

	if ((es = pAssocVar.openStatus()) != Acad::eOk)
		return es;

	name = pAssocVar->name();

	AcString errMsgEval(L"Expression evaluation failed");
	
	if((es = pAssocVar->evaluateExpression(value, errMsgEval)) != Acad::eOk)
		return es;
	
	expression = pAssocVar->expression();
	evaluatorId = pAssocVar->evaluatorId();

	return Acad::eOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Acad::ErrorStatus AcDbAssoc2dConstraintAPI::setVariableValue(
	const AcDbObjectId& variableId,
	const AcDbEvalVariant& value,
	const AcString& expression,
	const AcString& evaluatorId,
	AcString& errorMessage)
{
	Acad::ErrorStatus es;

	// Initialize Associative Framework
	if((es = AcDbAssocManager::initialize()) != Acad::eOk)
		return es;

	AcDbObjectPointer<AcDbAssocVariable> pAssocVar(variableId, kForWrite);

	if ((es = pAssocVar.openStatus()) != Acad::eOk)
		return es;

	if(evaluatorId.length() != 0)
	{
		if((es = pAssocVar->setEvaluatorId(evaluatorId)) != Acad::eOk)
			return es;
	}

	if(expression.length() != 0)
	{
		if((es = pAssocVar->validateNameAndExpression(pAssocVar->name(), expression, errorMessage)) != Acad::eOk)
			return es;

		if((es = pAssocVar->setExpression(expression, 
			EXPRESSION_EVALUATOR_DEFAULT, 
			true, 
			true, 
			errorMessage)) != Acad::eOk)
			return es;

		AcDbEvalVariant evaluatedExpressionValue;

		if((es = pAssocVar->evaluateExpression(evaluatedExpressionValue, errorMessage)) != Acad::eOk)
			return es;

		if((pAssocVar->setValue(evaluatedExpressionValue)) != Acad::eOk)
			return es;

		return es;
	}

	if((pAssocVar->setValue(value)) != Acad::eOk)
			return es;

	return es;
}

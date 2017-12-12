#pragma once

#include "AdnAssocConstraintAPI.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AcDbAssoc2dConstraintId
{
public:

	AcDbAssoc2dConstraintId() : mIndex(0) 
	{
	
	}

	bool isNull() const 
	{ 
		return mIndex == 0;//: 
	}

	AcDbObjectId BTRId() const; // Obtains the BTR from mConstraintGroupId

	AcDbObjectId mConstraintGroupId;

	unsigned mIndex;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AcDbAssoc2dConstraintAPI
{
public:

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a coincident geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createCoincidentConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which subentity point is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which subentity point is closest to it
		);
	 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a parallel geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createParallelConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a perpendicular geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createPerpendicularConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a concentric geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createConcentricConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a colinear geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createColinearConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a horizontal geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createHorizontalConstraint(
		AcDbObjectId& entId,	// ObjectID of first entity
		AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a vertical geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createVerticalConstraint(
		AcDbObjectId& entId,	// ObjectID of first entity
		AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a fix geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createFixConstraint(
		AcDbObjectId& entId,	// ObjectID of first entity
		AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a tangent geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createTangentConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a smooth geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createSmoothConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity which is a spline
		AcDbObjectId& entId2,	// ObjectID of second entity which can be a spline, line, arc, or polyline
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a symmetric geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createSymmetricConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity 
		AcDbObjectId& entSymId,	// ObjectID of symmetry entity 
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2,	// Point on second entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptSymEnt		// Point on symmetry entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a equal length geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createEqualLengthConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity 
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a equal radius geometric constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createEqualRadiusConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity
		AcDbObjectId& entId2,	// ObjectID of second entity 
		AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint (between 2 different entities)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity for aligned dimension
		AcGePoint3d& entPt1,	// Point on first entity used to determine first point sub entity for the aligned dimension
		AcDbObjectId& entId2,	// ObjectID of second entity for aligned dimension
		AcGePoint3d& entPt2,	// Point on second entity used to determine second point sub entity for the aligned dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint (between 2 vertices on the same entity)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		AcDbObjectId& entId,	// ObjectID of entity for aligned dimension
		AcGePoint3d& entPt1,	// Point on entity used to determine first point sub entity for the aligned dimension
		AcGePoint3d& entPt2,	// Point on entity used to determine second point sub entity for the aligned dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint on whole entity or on a segment of a Polyline
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		AcDbObjectId& entId,	// ObjectID of entity for aligned dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbSubentId subentId,	//subentity id, only used if entity is a polyline
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a 2 line angular dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus create2LineAngularDimConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity for angular dimension
		AcDbObjectId& entId2,	// ObjectID of second entity for angular dimension
		AcGePoint3d& entPt1,	// Point on first entity used to determine edge sub entity for the angular dimension
		AcGePoint3d& entPt2,	// Point on second entity used to determine edge sub entity for the angular dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an arc angular dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createArcAngularDimConstraint(
		AcDbObjectId& entId,	// ObjectID of arc based entity
		AcGePoint3d& entPt,		// Point on arc based entity
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a 3 point angular dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus create3PointAngularDimConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity for angular dimension
		AcDbObjectId& entId2,	// ObjectID of second entity for angular dimension
		AcDbObjectId& entId3,	// ObjectID of third entity for angular dimension
		AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the angular dimension
		AcGePoint3d& entPt2,	// Point on second entity used to determine second point sub entity for the angular dimension
		AcGePoint3d& entPt3,	// Point on third entity used to determine third point sub entity for the angular dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a radial dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createRadialDimConstraint(
		AcDbObjectId& entId,	// ObjectID of entity for radial dimension
		AcGePoint3d& entPt,		// Point on first entity used to determine arc sub entity for the radial dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a diameter dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createDiamDimConstraint(
		AcDbObjectId& entId,	// ObjectID of entity for radial dimension
		AcGePoint3d& entPt,		// Point on first entity used to determine arc sub entity for the radial dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an horizontal dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createHorizontalDimConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity for horizontal dimension
		AcDbObjectId& entId2,	// ObjectID of second entity for horizontal dimension
		AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the horizontal dimension
		AcGePoint3d& entPt2,	// Point on second entity used to determine point sub entity for the horizontal dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a vertical dimensional constraint
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createVerticalDimConstraint(
		AcDbObjectId& entId1,	// ObjectID of first entity for vertical dimension
		AcDbObjectId& entId2,	// ObjectID of second entity for vertical dimension
		AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the vertical dimension
		AcGePoint3d& entPt2,	// Point on second entity used to determine point sub entity for the vertical dimension
		AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
		);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Finds an existing AcDbAssocVariable of the given name in the scope of the given BTR
	//		Optionally creating a new empty one, if it does not exist yet
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus getVariableByName(
		const AcDbObjectId& btrId,	// Context
		const AcString& name,		// Variable name (must not be empty)
		bool  createIfDoesNotExist,	// Create the variable of the given name if not already exists
		AcDbObjectId& variableId);	// Returned ObjectId of the variable

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Renames an existing associative variable
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus renameVariable(
		const AcDbObjectId& variableId,	// ObjectId of the variable
		const AcString& newName,		// New name of the variable
		bool updateReferencingExpressions); //Specifies if all referencing expressions need to be updated immediately

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Returns properties about a specific associative variable 
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus getVariableValue(
		const AcDbObjectId& variableId,	// ObjectId of the variable
		AcString&           name,		// Current name of the variable
		AcDbEvalVariant&    value,		// Current value of the variable
		AcString&           expression, // Current expression of the variable
		AcString&           evaluatorId); // Current evaluatorId of the variable, if any

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Modifies an existing associative variable 
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus setVariableValue(
		const AcDbObjectId&    variableId,	// ObjectId of the variable
		const AcDbEvalVariant& value,		// new value of the var, can be omitted if expression is provided
		const AcString&        expression,	// new expression of the var, can be omitted if value is provided
		const AcString&        evaluatorId, // evaluatorId, can be omitted
		AcString&              errorMessage); // returned error message, if any
};



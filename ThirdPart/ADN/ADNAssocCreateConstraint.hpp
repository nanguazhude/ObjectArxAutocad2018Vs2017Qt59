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
	// 重合约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createCoincidentConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which subentity point is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which subentity point is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a parallel geometric constraint
	// 平行约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createParallelConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a perpendicular geometric constraint
	// 垂直约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createPerpendicularConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a concentric geometric constraint
	// 同心约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createConcentricConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a colinear geometric constraint
	// 共线约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createColinearConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a horizontal geometric constraint
	// 水平约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createHorizontalConstraint(
		const AcDbObjectId& entId,	// ObjectID of first entity
		const AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a vertical geometric constraint
	// 垂直约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createVerticalConstraint(
		const AcDbObjectId& entId,	// ObjectID of first entity
		const AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a fix geometric constraint
	// 固定约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createFixConstraint(
		const AcDbObjectId& entId,	// ObjectID of first entity
		const AcGePoint3d& ptEnt		// Point on first entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a tangent geometric constraint
	// 相切约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createTangentConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a smooth geometric constraint
	// 平滑约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createSmoothConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity which is a spline
		const AcDbObjectId& entId2,	// ObjectID of second entity which can be a spline, line, arc, or polyline
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a symmetric geometric constraint
	// 对称约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createSymmetricConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity 
		const AcDbObjectId& entSymId,	// ObjectID of symmetry entity 
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2,	// Point on second entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptSymEnt		// Point on symmetry entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a equal length geometric constraint
	// 相等约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createEqualLengthConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity 
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a equal radius geometric constraint
	// 相等约束
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createEqualRadiusConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity
		const AcDbObjectId& entId2,	// ObjectID of second entity 
		const AcGePoint3d& ptEnt1,	// Point on first entity used to determine which edge subentity is closest to it
		const AcGePoint3d& ptEnt2		// Point on second entity used to determine which edge subentity is closest to it
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint (between 2 different entities)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity for aligned dimension
		const AcGePoint3d& entPt1,	// Point on first entity used to determine first point sub entity for the aligned dimension
		const AcDbObjectId& entId2,	// ObjectID of second entity for aligned dimension
		const AcGePoint3d& entPt2,	// Point on second entity used to determine second point sub entity for the aligned dimension
		const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint (between 2 vertices on the same entity)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		const AcDbObjectId& entId,	// ObjectID of entity for aligned dimension
		const AcGePoint3d& entPt1,	// Point on entity used to determine first point sub entity for the aligned dimension
		const AcGePoint3d& entPt2,	// Point on entity used to determine second point sub entity for the aligned dimension
		const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an aligned dimensional constraint on whole entity or on a segment of a Polyline
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createAlignedDimConstraint(
		const AcDbObjectId& entId,	// ObjectID of entity for aligned dimension
		const AcGePoint3d& dimPos,	// Location of the dimension
		const AcDbSubentId subentId,	//subentity id, only used if entity is a polyline
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a 2 line angular dimensional constraint
	// 两条线夹角
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus create2LineAngularDimConstraint(
		const AcDbObjectId& entId1,	// ObjectID of first entity for angular dimension
		const AcDbObjectId& entId2,	// ObjectID of second entity for angular dimension
		const AcGePoint3d& entPt1,	// Point on first entity used to determine edge sub entity for the angular dimension
		const AcGePoint3d& entPt2,	// Point on second entity used to determine edge sub entity for the angular dimension
		const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an arc angular dimensional constraint
	// 圆弧角度
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createArcAngularDimConstraint(
        const AcDbObjectId& entId,	// ObjectID of arc based entity
        const AcGePoint3d& entPt,		// Point on arc based entity
        const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a 3 point angular dimensional constraint
	// 3点角度
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus create3PointAngularDimConstraint(
        const AcDbObjectId& entId1,	// ObjectID of first entity for angular dimension
        const AcDbObjectId& entId2,	// ObjectID of second entity for angular dimension
        const AcDbObjectId& entId3,	// ObjectID of third entity for angular dimension
        const AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the angular dimension
        const AcGePoint3d& entPt2,	// Point on second entity used to determine second point sub entity for the angular dimension
        const AcGePoint3d& entPt3,	// Point on third entity used to determine third point sub entity for the angular dimension
        const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a radial dimensional constraint
	// 半径
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createRadialDimConstraint(
        const AcDbObjectId& entId,	// ObjectID of entity for radial dimension
        const AcGePoint3d& entPt,		// Point on first entity used to determine arc sub entity for the radial dimension
        const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a diameter dimensional constraint
	// 直径
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createDiamDimConstraint(
		const AcDbObjectId& entId,	// ObjectID of entity for radial dimension
		const AcGePoint3d& entPt,		// Point on first entity used to determine arc sub entity for the radial dimension
		const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates an horizontal dimensional constraint
	// 水平线性尺寸
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createHorizontalDimConstraint(
        const AcDbObjectId& entId1,	// ObjectID of first entity for horizontal dimension
        const AcDbObjectId& entId2,	// ObjectID of second entity for horizontal dimension
        const AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the horizontal dimension
        const AcGePoint3d& entPt2,	// Point on second entity used to determine point sub entity for the horizontal dimension
        const AcGePoint3d& dimPos,	// Location of the dimension
		AcDbObjectId& varDimId	// ObjectId of the associated AssocVariable created (dimemsion parameter)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use: Creates a vertical dimensional constraint
	// 垂直线型尺寸
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static Acad::ErrorStatus createVerticalDimConstraint(
        const AcDbObjectId& entId1,	// ObjectID of first entity for vertical dimension
        const AcDbObjectId& entId2,	// ObjectID of second entity for vertical dimension
        const AcGePoint3d& entPt1,	// Point on first entity used to determine point sub entity for the vertical dimension
        const AcGePoint3d& entPt2,	// Point on second entity used to determine point sub entity for the vertical dimension
        const AcGePoint3d& dimPos,	// Location of the dimension
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



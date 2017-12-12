#pragma once
#include <object_arx_global.hpp>

class AdnConstraintEvaluationCallback :
	public AcDbAssocEvaluationCallback
{
	AcDbFullSubentPathArray _fixedGeom;

	AcDbEvalContext* _evalContext;

public:
	AdnConstraintEvaluationCallback(void);
	~AdnConstraintEvaluationCallback(void);

	virtual void beginActionEvaluation          (AcDbAssocAction*) {}
	virtual void endActionEvaluation            (AcDbAssocAction*) {}

	virtual void setActionEvaluationErrorStatus (AcDbAssocAction*    pAction,
		Acad::ErrorStatus   errorStatus,
		const AcDbObjectId& objectId   = AcDbObjectId::kNull,
		AcDbObject*         pObject    = NULL,
		void*               pErrorInfo = NULL){}

	virtual void getActionEvaluationErrorStatus (AcDbAssocAction*   pAction,
		Acad::ErrorStatus& errorStatus,
		AcDbObjectId&      objectId,
		AcDbObject*&       pObject,
		void*&             pErrorInfo) {}

	virtual void beginActionEvaluationUsingObject(AcDbAssocAction*    pAction, 
		const AcDbObjectId& objectId, 
		bool                objectIsGoingToBeUsed,
		bool                objectIsGoingToBeModified,
		AcDbObject*&        pSubstituteObject) {}

	virtual void endActionEvaluationUsingObject  (AcDbAssocAction*    pAction, 
		const AcDbObjectId& objectId, 
		AcDbObject*         pObject) {}

	virtual AcDbEvalContext*                      getAdditionalData() const;


	virtual bool isDragging                      () { return false; }
	virtual bool cancelActionEvaluation          () { return false; }

	void addTempFixedSubEnts(const AcDbFullSubentPathArray &fixedGeom);
};


#include "AdnConstraintEvaluationCallback.hpp"

AdnConstraintEvaluationCallback::AdnConstraintEvaluationCallback(void)
{
	_evalContext = NULL;
}

AdnConstraintEvaluationCallback::~AdnConstraintEvaluationCallback(void)
{
	delete _evalContext;
}

void AdnConstraintEvaluationCallback::addTempFixedSubEnts(
	const AcDbFullSubentPathArray &fixedGeom)
{
	if(_evalContext == NULL)
		_evalContext = new AcDbEvalContext();

	_fixedGeom.append(fixedGeom);
}

AcDbEvalContext* AdnConstraintEvaluationCallback::getAdditionalData() const
{
	if(_evalContext == NULL)
		return NULL;

	const ACHAR* key = L"TempFixedConsGeoms";

	AcDbEvalContextPair pair(key, (void*)(&_fixedGeom));

	_evalContext->insertAt(pair);

	return _evalContext;
}

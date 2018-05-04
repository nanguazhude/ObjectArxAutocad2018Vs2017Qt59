(setq ss (ssget))
(vl-cmdf "-objectscale" ss "" "add" (getvar "cannoscale") "")

////////////////////////////////////////////////////////////////////////////////////
//Use: Create annotative entity
//
////////////////////////////////////////////////////////////////////////////////////
void CreateAnnotative()
{
	Acad::ErrorStatus es;

	ads_name name;
	ads_point point;

	if( acedEntSel(L"\nSelect an entity...", name, point) != RTNORM)
	{
		acutPrintf(L"Failed to select entity...");
		return;
	}

	AcDbObjectId idEnt;
	acdbGetObjectId(idEnt, name);

	AcDbEntity* pEntity = NULL;
	if( acdbOpenAcDbEntity(pEntity, idEnt,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"Failed to open entity...");
		return;
	}

	// see if we are annotative
	AcDbAnnotativeObjectPE *annotationPE = ACRX_PE_PTR(pEntity, AcDbAnnotativeObjectPE);

	if (annotationPE && annotationPE->annotative(pEntity))
	{
		acutPrintf(L"Entity already annotative...");
		pEntity->close();
		return;
	}

	if ((es=annotationPE->setAnnotative(pEntity, true)) != Acad::eOk)
	{
		acutPrintf(L"Failed to set entity annotative...");
		pEntity->close();
		return;
	}

	pEntity->close();
}

////////////////////////////////////////////////////////////////////////////////////
//Use: Add a new or existing scale to an existing annotative entity
//
////////////////////////////////////////////////////////////////////////////////////
void AddAnnotativeScale(
	const ACHAR* ScaleName, 
	const double& PaperUnits, 
	const double& DrawingUnits)
{
	Acad::ErrorStatus es;

	ads_name name;
	ads_point point;

	if( acedEntSel(L"\nSelect an entity...", name, point) != RTNORM)
	{
		acutPrintf(L"Failed to select entity...");
		return;
	}

	AcDbObjectId idEnt;
	acdbGetObjectId(idEnt, name);

	AcDbEntity* pEntity = NULL;
	if( acdbOpenAcDbEntity(pEntity, idEnt,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"Failed to open entity...");
		return;
	}

	// see if we are annotative
	AcDbAnnotativeObjectPE *annotationPE = ACRX_PE_PTR(pEntity, AcDbAnnotativeObjectPE);

	if (!annotationPE || !annotationPE->annotative(pEntity))
	{
		acutPrintf(L"Entity not annotative...");
		pEntity->close();
		return;
	}

    AcDbObjectContextManager *contextManager = 
		acdbHostApplicationServices()->workingDatabase()->objectContextManager();
    
    if (contextManager)
    {
		// now get the Annotation Scaling context collection (named ACDB_ANNOTATIONSCALES_COLLECTION)
		AcDbObjectContextCollection* const contextCollection = 
		contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

		if (contextCollection) 
		{
			AcDbAnnotationScale annotationScale;
			AcDbObjectContext* pScaleContext = NULL;

			//Check if context already exist
			if (contextCollection->hasContext(ScaleName))
			{
				pScaleContext = contextCollection->getContext(ScaleName);
			}

			// create a brand new scale context
			else
			{
				annotationScale.setName(ScaleName);
				annotationScale.setPaperUnits(PaperUnits);
				annotationScale.setDrawingUnits(DrawingUnits);

				// now add to the drawing's context collection
				if( (es = contextCollection->addContext(&annotationScale)) != Acad::eOk)
				{
					acutPrintf(L"\nFailed to add scale to context...");
					pEntity->close();
					return;
				}

				pScaleContext = &annotationScale;
			}

			// now get the Object Context PEX
			AcDbObjectContextInterface *objectContextInterface = ACRX_PE_PTR(pEntity, AcDbObjectContextInterface);
			
			if (objectContextInterface) 
			{
				// add My scale context to the list of contexts
				if( (es = objectContextInterface->addContext(pEntity, *pScaleContext)) != Acad::eOk)
				{
					acutPrintf(L"\nFailed to add scale to entity...");
					pEntity->close();
					return;
				}
			}
		}
	}

	pEntity->close();
	return;
}

////////////////////////////////////////////////////////////////////////////////////
//Use: Remove a scale from an existing annotative entity
//
////////////////////////////////////////////////////////////////////////////////////
void RemoveAnnotativeScale(const ACHAR* ScaleName)
{
	Acad::ErrorStatus es;

	ads_name name;
	ads_point point;

	if( acedEntSel(L"\nSelect an entity...", name, point) != RTNORM)
	{
		acutPrintf(L"Failed to select entity...");
		return;
	}

	AcDbObjectId idEnt;
	acdbGetObjectId(idEnt, name);

	AcDbEntity* pEntity = NULL;
	if( acdbOpenAcDbEntity(pEntity, idEnt,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"Failed to open entity...");
		return;
	}

	// see if we are annotative
	AcDbAnnotativeObjectPE *annotationPE = ACRX_PE_PTR(pEntity, AcDbAnnotativeObjectPE);

	if (!annotationPE || !annotationPE->annotative(pEntity))
	{
		acutPrintf(L"Entity not annotative...");
		pEntity->close();
		return;
	}

	AcDbObjectContextInterface *objectContextInterface = ACRX_PE_PTR(pEntity, AcDbObjectContextInterface);
	
	if (objectContextInterface) 
	{
		AcDbObjectContextManager *contextManager = 
		acdbHostApplicationServices()->workingDatabase()->objectContextManager();

		if (contextManager)
		{
			AcDbObjectContextCollection* const contextCollection = 
			contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

			if (contextCollection) 
			{
				//Check if context exists
				if (contextCollection->hasContext(ScaleName))
				{
					AcDbObjectContext* pScaleContext = contextCollection->getContext(ScaleName);

					if (objectContextInterface->hasContext(pEntity, *pScaleContext))
					{
						if( (es = objectContextInterface->removeContext( pEntity, *pScaleContext)) == Acad::eOk)
						{
							acutPrintf(L"\nAnnotation Scale %s removed from entity...", ScaleName);
						}
					}
				}
				
			}
		}
	}

	pEntity->close();
	return;
}

////////////////////////////////////////////////////////////////////////////////////
//Use: Display the list of annotative scales for the selected entity
//
////////////////////////////////////////////////////////////////////////////////////
void DisplayAnnotativeScales()
{
	Acad::ErrorStatus es;

	ads_name name;
	ads_point point;

	if( acedEntSel(L"\nSelect an entity...", name, point) != RTNORM)
	{
		acutPrintf(L"Failed to select entity...");
		return;
	}

	AcDbObjectId idEnt;
	acdbGetObjectId(idEnt, name);

	AcDbEntity* pEntity = NULL;
	if( acdbOpenAcDbEntity(pEntity, idEnt,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"Failed to open entity...");
		return;
	}

	// see if we are annotative
	AcDbAnnotativeObjectPE *annotationPE = ACRX_PE_PTR(pEntity, AcDbAnnotativeObjectPE);

	if (!annotationPE || !annotationPE->annotative(pEntity))
	{
		acutPrintf(L"Entity is not annotative...");
		pEntity->close();
		return;
	}

	// try to extract the Object Context Protocol Extension Object
	AcDbObjectContextInterface *objectContextInterface = ACRX_PE_PTR(pEntity, AcDbObjectContextInterface);

	// if we have an Annotation Scaling context interface
	if (objectContextInterface) 
	{
		// next get the objectContextManager
		AcDbObjectContextManager *contextManager = 
		acdbHostApplicationServices()->workingDatabase()->objectContextManager();

		if (contextManager)
		{
			// now get the annotation scales collection context
			AcDbObjectContextCollection* const contextCollection = 
			contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

			if (contextCollection) 
			{
				// lets iterate through them, create a new iterator
				AcDbObjectContextCollectionIterator* pIter = contextCollection->newIterator();

				for (pIter->start(); !pIter->done(); pIter->next()) 
				{
					AcDbObjectContext *objectContext = NULL;
					// get the context object for this iteration
					Acad::ErrorStatus es = pIter->getContext(objectContext);

					// get the current annotation scale
					AcDbAnnotationScale *annotationScale = AcDbAnnotationScale::cast(objectContext);

					// check to see if our object supports this scale
					if (annotationScale && objectContextInterface->hasContext(pEntity, *annotationScale))
					{
						double scale;
						annotationScale->getScale(scale);

						double paper;
						annotationScale->getPaperUnits(paper);
						
						AcString ScaleName;
						annotationScale->getName(ScaleName);

						acutPrintf(L"\n - %s (%.5f)", ScaleName.kACharPtr(), scale); 
					}
				}

				delete pIter;
			}
		}
	}
	
	pEntity->close();
}



//getScaleListDictionary
//AcDbAnnotationScale

/***/
LTSCALE = 1.0
CELTSCALE = 1.0
PSLTSCALE = 1
MSLTSCALE = 1
https://cadsetterout.com/autocad-tutorials/line-type-scales/

/**/
   AcString annoName(L"1:2);
   FME_Real64 paperUnits = 1.0;
   FME_Real64 drawingUnits = 2.0;

   AcDbObjectContextManager* contextMgr = pDb_->objectContextManager();
   AcDbObjectContextCollection* contextCollection =
      contextMgr->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

   if(!contextCollection->hasContext(annoName))
   {
      // basic scale object on stack, to add
      AcDbAnnotationScale annoScaleToAdd;
      acadErr = annoScaleToAdd.setName(annoName);
      acadErr = annoScaleToAdd.setPaperUnits(paperUnits);
      acadErr = annoScaleToAdd.setDrawingUnits(drawingUnits);

      // add annotation scale to overall database contexts
      acadErr = contextCollection->addContext(&annoScaleToAdd);
   }
   // else found existing annotation scale, so use that definition

   // either existing by name, or newly created by properties
   if(contextCollection->hasContext(annoName))
   {
      AcDbObjectContext* annoContext = contextCollection->getContext(annoName))

      // AcDbAnnotationScale is a subclass of AcDbObjectContext
      AcDbAnnotationScale* annoScale = dynamic_cast<AcDbAnnotationScale*>(annoContext);

      acadErr = pViewport->setAnnotationScale(annoScale);
   }


/**/
static void CheckAnnotative(void)
{
    ads_name en; ads_point p;
    if (acedEntSel(_T("\nSelect annotative entity: "),en,p) != RTNORM) return;
    AcDbObjectId eid; acdbGetObjectId(eid,en);
    AcDbEntityPointer pEnt(eid,AcDb::kForRead);
    if (pEnt.openStatus() != Acad::eOk) return;
    AcDbAnnotativeObjectPE *ann = ACRX_PE_PTR(pEnt, AcDbAnnotativeObjectPE);
    if (ann && ann->annotative(pEnt)) {
      AcDbObjectContextInterface *objInt = ACRX_PE_PTR(pEnt, AcDbObjectContextInterface);
      if (objInt) {
          AcDbObjectContextManager *cManager = acdbCurDwg()->objectContextManager();
          if (cManager) {
              AcDbObjectContextCollection *col = 
                      cManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);
              if (col) {
                    AcDbObjectContextCollectionIterator* iter = col->newIterator();
                    if (iter) {
                        for (iter->start(); !iter->done(); iter->next()) {
                            AcDbObjectContext *cont = NULL; iter->getContext(cont);
                            if (cont) {
                                AcDbAnnotationScale *pScale = AcDbAnnotationScale::cast(cont);
                                if (pScale && objInt->hasContext(pEnt,*pScale)){
                                    double scale  = 1; pScale->getScale(scale);
                                    double pUnits = 1; pScale->getPaperUnits(pUnits);
                                    double dUnits = 1; pScale->getDrawingUnits(dUnits);
                                    AcString name; pScale->getName(name);
                                    acutPrintf(_T("\nName = <%s> Scale = %g PaperUnits = %g DrawingUnits= %g"), 
                                        name.kACharPtr(), scale, pUnits, dUnits);
                                }
                            }
                        }
                        delete iter;
                    }
              }
          }
      }
    } else {
        acutPrintf(_T("\nNot annotative entity!"));
    }

}




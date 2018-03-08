﻿varAns.emplace(LR"(双点划线)"sv, ApplyMaps::value_type::second_type{ [](simple_code_args) {
	sstd::ArxClosePointer<AcDbLayerTableRecord> varLocalLTR;
	if (argLTR == nullptr) {
		varLocalLTR = new AcDbLayerTableRecord;
		argLTR = varLocalLTR;
		argLTR->setName(argNM.data());
	}

	/*初始化颜色*****************************************************/
	AcCmColor varLColor;
	varLColor.setColorIndex(6)/*洋红*/;
	/*初始化线宽*****************************************************/
	AcDb::LineWeight varLWeight = AcDb::kLnWt015;
	/*设置透明度*****************************************************/
	AcCmTransparency varLTP;
	varLTP.setAlphaPercent(1.0)/*1.0代表不透明，0.0代表透明*/;
	/*初始化线型*****************************************************/
	std::optional<AcDbObjectId> varLTypeID;
	{
		AcDbObjectId varLTypeIDTmp;
		if (Acad::eOk == argLTT->getAt(LR"(_双点划线_)", varLTypeIDTmp)) {
			varLTypeID = varLTypeIDTmp;
		}else {
			acutPrintf(LR"(找不到线型：_双点划线_
)");
		}
	}
	/******************************************************/
	argLTR->setIsOff(false)/*打开/关闭*/;
	argLTR->setIsFrozen(false)/*冻结*/;
	argLTR->setIsLocked(false)/*锁定*/;
	argLTR->setColor(varLColor)/*颜色*/;
	argLTR->setIsPlottable(true)/*打印*/;
	if(argIsPlotNS /*||varLocalLTR*/ )argLTR->setPlotStyleName(LR"(Style 1)");
	
	if (varLTypeID) {
		argLTR->setLinetypeObjectId(*varLTypeID)/*设置线型*/;
	}
	argLTR->setLineWeight(varLWeight)/*线宽*/;
	argLTR->setTransparency(varLTP)/*透明度*/;

	if (varLocalLTR) {
		argLT->add(argLTR);
	}
	argLTR->setDescription(LR"(图层：双点划线)")/*注释*/;
} ,false });



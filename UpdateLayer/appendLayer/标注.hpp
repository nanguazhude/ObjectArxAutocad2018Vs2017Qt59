varAns.emplace(LR"(标注)"sv, ApplyMaps::value_type::second_type{ [](simple_code_args) {
	sstd::ArxClosePointer<AcDbLayerTableRecord> varLocalLTR;
	if (argLTR == nullptr) {
		varLocalLTR = new AcDbLayerTableRecord;
		argLTR = varLocalLTR;
		argLTR->setName(argNM.data());
	}

	/*初始化颜色*****************************************************/
	AcCmColor varLColor;
	varLColor.setColorIndex(3)/*绿色*/;
	/*初始化线宽*****************************************************/
	AcDb::LineWeight varLWeight = AcDb::kLnWt015;
	/*设置透明度*****************************************************/
	AcCmTransparency varLTP;
	varLTP.setAlphaPercent(1.0)/*1.0代表不透明，0.0代表透明*/;
	/*初始化线型*****************************************************/
	std::optional<AcDbObjectId> varLTypeID;
	{
		AcDbObjectId varLTypeIDTmp;
		if (Acad::eOk == argLTT->getAt(LR"(Continuous)", varLTypeIDTmp)) {
			varLTypeID = varLTypeIDTmp;
		}else {
			acutPrintf(LR"(找不到线型：Continuous
)");
		}
	}
	/******************************************************/
	argLTR->setIsOff(false)/*打开/关闭*/;
	argLTR->setIsFrozen(false)/*冻结*/;
	argLTR->setIsLocked(false)/*锁定*/;
	argLTR->setColor(varLColor)/*颜色*/;
	argLTR->setIsPlottable(true)/*打印*/;
	argLTR->setDescription(LR"(图层：标注(兼容旧图纸))")/*注释*/;
	if (varLTypeID) {
		argLTR->setLinetypeObjectId(*varLTypeID)/*设置线型*/;
	}
	argLTR->setLineWeight(varLWeight)/*线宽*/;
	argLTR->setTransparency(varLTP)/*透明度*/;

	if (varLocalLTR) {
		argLT->add(argLTR);
	}

} ,false });



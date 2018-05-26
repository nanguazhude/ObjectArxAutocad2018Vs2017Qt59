$Functions.emplace(LR"(Standard)"s,
	Functions::value_type::second_type{ [](_UpdateMLeaderStyle*argTable,
	const std::wstring &argName,
	AcDbMLeaderStyle *argR) {
	sstd::ArxClosePointer<AcDbMLeaderStyle> varLocal;
	if (argR == nullptr) {
		AcDbObjectId varID;
		argR = new AcDbMLeaderStyle;
		varLocal = argR;
		argR->setName(argName.c_str());
	}
	/****************************************************************/
	argR->setBreakSize(0)/**/;
	argR->setAnnotative(true)/*注释性*/;
	argR->setTextColor(40_ac);
	argR->setLeaderLineColor(11_ac);

auto varDoFunction = [argR,argTable]() {
	argR->setTextHeight(zihao_shx(6.75));
	argR->setArrowSize(4.6)/*箭头大小*/;
	argR->setMaxLeaderSegmentsPoints(2);
	//argR->setDescription(LR"(Hellow Kitty!)");
	argR->setContentType(AcDbMLeaderStyle::kNoneContent);
	if (argTable->$TextTypeID) argR->setTextStyleId(*argTable->$TextTypeID);
	//argR->setTextAngleType(AcDbMLeaderStyle::kHorizontalAngle);
	//argR->setTextAttachmentDirection(AcDbMLeaderStyle::kAttachmentHorizontal);
	//argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomOfTopLine, AcDbMLeaderStyle::kLeftLeader);
	//argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomOfTopLine, AcDbMLeaderStyle::kRightLeader);
	//argR->setExtendLeaderToText(true);
	//argR->setLandingGap(1.2);
	argR->setEnableDogleg(false);
	if (argTable->$ArrowID) {
		argR->setArrowSymbolId(*(argTable->$ArrowID))/*箭头样式*/;
	}
	else { 
		argR->setArrowSymbolId(LR"()")/*箭头样式*/;
	}
};
	
	if constexpr( Version == 0 ) {
		varDoFunction();
	}
	else {
		if (varLocal) {
			varDoFunction();
		}
	}

	AcDbObjectId varID;
	//argR->postMLeaderStyleToDb(argTable->$DB, argName.c_str(), varID);
	if (varLocal) {
		argTable->$MleaderTable->setAt(argName.c_str(),
						argR, varID);
	}
	/****************************************************************/
} ,false });


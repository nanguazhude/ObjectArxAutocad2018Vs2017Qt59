$Functions.emplace(LR"(无箭头引线)"s,
	Functions::value_type::second_type{ [](_UpdateMLeaderStyle*argTable,
	const std::wstring &argName,
	AcDbMLeaderStyle *argR) {
	sstd::ArxClosePointer<AcDbMLeaderStyle> varLocal;
	if (argR == nullptr) {
		AcDbObjectId varID;
		argR = new AcDbMLeaderStyle;
		varLocal = argR;
		argR->setName(argName.data());
		argTable->$MleaderTable->setAt(argName.data(),
			argR, varID);
	}
	/****************************************************************/
	argR->setAnnotative(true)/*注释性*/;
	argR->setArrowSize(4.6)/*箭头大小*/;
	argR->setMaxLeaderSegmentsPoints(2);
	argR->setBreakSize(0)/**/;
	//argR->setDescription(LR"(无箭头引线)");
	argR->setContentType(AcDbMLeaderStyle::kMTextContent);
	if (argTable->$TextTypeID) argR->setTextStyleId(*argTable->$TextTypeID);
	argR->setTextAngleType(AcDbMLeaderStyle::kHorizontalAngle);
	argR->setTextAttachmentDirection(AcDbMLeaderStyle::kAttachmentHorizontal);
	argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kLeftLeader);
	argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kRightLeader);
	argR->setExtendLeaderToText(true);
	argR->setLandingGap(1.2);
	argR->setEnableDogleg(false);
	argR->setArrowSymbolId(LR"(_NONE)")/*箭头样式*/;
	argR->setTextColor(40_ac);
	argR->setLeaderLineColor(11_ac);
	/****************************************************************/
} ,false });


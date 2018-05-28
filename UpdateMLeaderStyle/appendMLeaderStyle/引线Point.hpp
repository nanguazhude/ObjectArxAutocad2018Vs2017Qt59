$Functions.emplace(LR"(引线Point)"s,
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
	argR->setTextColor(40_ac);
	argR->setLeaderLineColor(31_ac);
	argR->setAnnotative(true)/*注释性*/;
	argR->setBreakSize(0)/**/;

	auto varDoFunction = [argR,argTable]() {
		argR->setTextHeight(zihao_shx(6.75));
		argR->setArrowSize(1.6)/*点大小*/;
		argR->setMaxLeaderSegmentsPoints(2);
		//argR->setDescription(LR"(Hellow Kitty!)");
		argR->setContentType(AcDbMLeaderStyle::kMTextContent);
		if (argTable->$TextTypeID) argR->setTextStyleId(*argTable->$TextTypeID);
		argR->setTextAngleType(AcDbMLeaderStyle::kHorizontalAngle);
		argR->setTextAttachmentDirection(AcDbMLeaderStyle::kAttachmentHorizontal);
		argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kLeftLeader);
		argR->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomLine, AcDbMLeaderStyle::kRightLeader);
		argR->setExtendLeaderToText(true);
		argR->setLandingGap(1.2);
		argR->setEnableDogleg(false);
		argR->setArrowSymbolId(LR"(_DOT)")/*箭头样式*/;
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
	//argR->postMLeaderStyleToDb(argTable->$DB, argName.c_str(),varID);
	if (varLocal) {
		argTable->$MleaderTable->setAt(argName.c_str(),
						argR, varID);
	}
	/****************************************************************/
} ,false });


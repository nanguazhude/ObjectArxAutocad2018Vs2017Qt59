varAns.emplace(LR"(宋体)"sv, ApplyMaps::value_type::second_type{
	[](simple_code_args) {
	sstd::ArxClosePointer<AcDbTextStyleTableRecord> varLocalR;
	if (argR == nullptr) {
		argR = new AcDbTextStyleTableRecord;
		varLocalR = argR;
		argR->setName(argNM.data());
		argTST->add(argR);
	}

	argR->setFlagBits(argR->flagBits()&0b11111001)/*清除标志位*/;
	argR->setIsShapeFile(false)/**/;
	argR->setFont(LR"(宋体)",
		false,
		false,
		kChineseSimpCharset,
		Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kFixed,
		Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kModern
	);

	auto varSetTextHeight = [argR]() {
		argR->setTextSize( zihao(0.0) )/*文字高度*/;
		if (argR->priorSize()<zihao(5)){
			argR->setPriorSize(zihao(5.7))/*默认大小*/;
		}
	};

    if constexpr(Version == 0) {
		varSetTextHeight();
	}
	else {
		if (varLocalR) {
			varSetTextHeight();
		}
	}

	argR->setXScale(1.0)/*宽度比*/;
	setAnnotative(argR,false)/*注释性*/;

},false });



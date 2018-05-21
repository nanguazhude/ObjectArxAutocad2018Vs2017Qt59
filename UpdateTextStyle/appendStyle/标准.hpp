varAns.emplace(LR"(标准)"sv, ApplyMaps::value_type::second_type{
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
	argR->setFileName(LR"(txt.shx)")/*shx字体文件名*/;
	argR->setBigFontFileName(LR"(gbcbig.SHX)")/*shx大字体文件名*/;
	auto varSetTextHeight = [argR]() {
	argR->setTextSize(zihao_shx(0.0))/*文字高度*/;
	if (argR->priorSize()<zihao_shx(5))argR->setPriorSize(zihao_shx(6.75))/*默认大小*/;
};
	if constexpr(Version == 0) {
		varSetTextHeight();
	}
	else {
		if (varLocalR) {
			varSetTextHeight();
		}
	}

	argR->setXScale(0.8)/*宽度比*/;
	setAnnotative(argR,true)/*注释性*/;

},false });



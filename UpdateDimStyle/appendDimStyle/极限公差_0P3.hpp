varAns.emplace(LR"(极限公差_0P3)", ApplyMaps::value_type::second_type{
	[](simple_code_args,
	StyleTableRecord * arg0,StyleTableRecord * arg2,
	StyleTableRecord * arg3,StyleTableRecord * arg4,
	ThisState * argGl) {
	/*https://knowledge.autodesk.com/zh-hans/support/autocad/troubleshooting/caas/sfdcarticles/sfdcarticles/CHS/Creating-a-new-dimension-style-on-the-command-line.html*/
/******************************************************************/
	sstd::ArxClosePointer<StyleTableRecord> varLocalR;
	if (argR == nullptr) {
		argR = new StyleTableRecord;
		varLocalR = argR;
		argR->setName(argNM.data());
		argTST->add(argR);
	}

	/*线*******************************/
	{
		argR->setDimclrd(102_ac)/*尺寸线颜色*/;
		argR->setDimclre(102_ac)/*尺寸界限颜色*/;
		argR->setDimdli(7.0)/*基线间距*/;
		argR->setDimexe(2.0)/*尺寸界限超出尺寸线距离*/;
		argR->setDimexo(0.0);
	}
	/*符号和箭头***********************/
	{
		constexpr const auto varRowInit = LR"(_MY_ROW)"sv;
		const auto varRowType = argGl->getBlock(varRowInit);
		if (varRowType) {
			argR->setDimblk(*varRowType);
			argR->setDimblk1(*varRowType)/*第一个箭头*/;
			argR->setDimblk2(*varRowType)/*第二个箭头*/;
		}
		else {
			argR->setDimblk(LR"()");
			argR->setDimblk1(LR"()")/*第一个箭头*/;
			argR->setDimblk2(LR"()")/*第二个箭头*/;
		}
		argR->setDimasz(4.6)/*箭头长度*/;
		argR->setDimcen(0.0)/*圆心标记*/;
	}
	/*文字****************************/
	{
		const auto varTextType = argGl->getTextStyle(LR"(@Standard)");
		if (varTextType) { argR->setDimtxsty(*varTextType); }
		argR->setDimclrt(111_ac);
		argR->setDimgap(1.25)/*尺寸线和文字的间距*/;
		argR->setDimtfill(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-4E38E29F-DE85-4791-A2E7-4DC22842B1B4-htm.html*/;
		argR->setDimtad(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-60D1241D-CEA7-4493-BD6A-4EF433F3C946-htm.html*/;
		argR->setDimjust(0)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-C67348A9-2260-4135-A7FF-FE0B45211CB0-htm.html*/;
		/*文在在尺寸界限内时对齐尺寸线,在尺寸线外时水平对齐*/
		argR->setDimtih(false);
		argR->setDimtoh(true);
	}
	/*调整****************************/
	{
		setAnnotative(argR, true)/*注释性*/;
		argR->setDimatfit(1)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-2F8FA7E7-C79F-40EA-A416-C4B02D09F310-htm.html*/;
		argR->setDimtofl(true)/*强制绘制尺寸线*/;
	}
	/*主单位**************************/
	{
		argR->setDimlunit(2)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-2ECDF7CF-6EEA-4174-B50C-8630D5002C20-htm.html*/;
		argR->setDimdec(2)/*小数0.00*/;
		argR->setDimdsep(wchar_t('.'))/*小数分隔符*/;
		argR->setDimaunit(0)/*十进制度数*/;
		argR->setDimadec(2)/*0.00*/;
		argR->setDimzin(8)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-A1860981-FE1C-4947-927B-7CD6B8CEF8EE-htm.html*/;
		argR->setDimazin(2)/*https://knowledge.autodesk.com/zh-hans/support/autocad/learn-explore/caas/CloudHelp/cloudhelp/2018/CHS/AutoCAD-Core/files/GUID-69E9BFD3-06A4-468D-88F0-7C9741407194-htm.html*/;
	}
	/*换算单位*/
	{
	}
	/*公差*/
	{
		argR->setDimtol(true)/*不显示公差*/;
		argR->setDimtdec(2)/*小数0.00*/;
		argR->setDimtfac(0.75)/*公差高度比*/;
		argR->setDimtp(0.3)/*公差最大值*/;
		argR->setDimtm(0.0)/*公差最小值*/;
		argR->setDimtzin(8)/*公差消零*/;
		argR->setDimalttz(8)/*公差消零*/;
		argR->setDimtolj(1)/*公差显示位置*/;
	}

	/******************************************************************/
	{
		_linear_child(argNM, argTST, argR, arg0);
		_angular_child(argNM, argTST, argR, arg2);
		_diameter_child(argNM, argTST, argR, arg3);
		_radius_child(argNM, argTST, argR, arg4);
	}
},false });




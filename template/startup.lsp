(defun c:zoomo() (command "zoom" "o" pause) )
;(defun c:dd()   (command "dim" ) )
(defun c:ddf()  (command "dimlinear" ) )
(defun c:ddfa() (command "dimaligned" ) )

(defun c:zrec( / varLY varS )

  (setq varLY(getvar "clayer") )
  (setvar "CMDECHO" 0 )
  (command "-layer" "on" "参考线" "" )
  (command "clayer" "参考线")

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "CMDECHO" 1 )
     (setvar "clayer" varLY )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )
;
  (command "rectang" pause pause );
  (setq varS (  ssget "l" ) )
  (command "AUTOCONSTRAIN" (ssget )  "" )
  (command "BCONSTRUCTION" varS "" "C" ) 
  
  (normalReturn_this)

)

(defun c:zarc()
  (command "arc" pause
	   "e"   pause
	   "d" 
	   )
)

(defun c:aaq(/ varS)
	(setq varS(ssget ":E"))
	(if (/= varS nil)(progn
		(command "AUTOCONSTRAIN" varS "" )
	))
)

(defun c:szr(/ varS)                                    ;选择尺寸线
  (command "pselect" "" )                               ;取消全部选择
  (setq varS (ssget  (list (cons -4 "<or") 
  (cons 8 "参考点" ) 
  (cons 8 "参考线" )
  (cons 8 "临时图形" )
  (cons -4 "or>") ) ) )                                 ;选择集合  
  (command "pselect" varS "" )                          ;选择
)

(defun c:szs(/ varS)                                    ;选择尺寸线
  (command "pselect" "" )                               ;取消全部选择
  (setq varS (ssget  (list (cons 8 "尺寸线" ) ) ) )     ;选择集合  
  (command "pselect" varS "" )                          ;选择
)

(defun c:szd(/ varS)                                  ;选择虚线
  (command "pselect" "" )                             ;取消全部选择
  (setq varS (ssget  (list (cons 8 "虚线" ) ) ) )     ;选择集合  
  (command "pselect" varS "" )                        ;选择
)

(defun c:szz(/ varS)                                  ;选择0
  (command "pselect" "" )                             ;取消全部选择
  (setq varS (ssget  (list (cons 8 "0" ) ) ) )        ;选择集合  
  (command "pselect" varS "" )                        ;选择
)

(defun c:szdp(/ varS)                                  ;选择Defpoints
  (command "pselect" "" )                              ;取消全部选择
  (setq varS (ssget  (list (cons 8 "Defpoints" ) ) ) ) ;选择集合  
  (command "pselect" varS "" )                         ;选择
)

(defun c:szc(/ varS)                                   ;选择中心线
  (command "pselect" "" )                              ;取消全部选择
  (setq varS (ssget  (list (cons 8 "中心线" ) ) ) )    ;选择集合  
  (command "pselect" varS "" )                         ;选择
)

(defun c:szf(/ varS)                                   ;选择细实线
  (command "pselect" "" )                              ;取消全部选择
  (setq varS (ssget  (list (cons 8 "细实线" ) ) ) )    ;选择集合  
  (command "pselect" varS "" )                         ;选择
)

(defun c:szt(/ varS)                                    ;选择粗实线
  (command "pselect" "" )                               ;取消全部选择
  (setq varS (ssget  (list (cons 8 "粗实线*" ) ) ) )     ;选择集合  
  (command "pselect" varS "" )                          ;选择
)

;(defun c:test111(/ varS)
;  (command "pselect" "" )     ;取消全部选择
;  (setq varS (ssget "A") )    ;选择集合
;  (command "pselect" varS "" );选择
;)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zx毂形位公差(/ varLY  )
  (setq varLY(getvar "clayer") )
  (setvar "CMDECHO" 0 )
  (command "-layer" "on" "细实线" "" )
  (command "clayer" "细实线")

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "CMDECHO" 1 )
     (setvar "clayer" varLY )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (command "insert" "@毂形位公差.111133" pause "1" "1" "0"   )
  (normalReturn_this)

)
;@刹车面形位公差.111133
(defun c:zx刹车面形位公差(/ varLY  )
  (setq varLY(getvar "clayer") )
  (setvar "CMDECHO" 0 )
  (command "-layer" "on" "细实线" "" )
  (command "clayer" "细实线")

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "CMDECHO" 1 )
     (setvar "clayer" varLY )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (command "insert" "@刹车面形位公差.111133" pause "1" "1" "0"   )
  (normalReturn_this)

)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;(list (cons -4   "<or") (cons 8 "参考点") (cons 8 "参考线")  (cons 8 "临时图形") (cons -4 "or>") )
(defun c:zdo(/ varS)
  (c:zgdns) ;隐藏所有约束
  ;隐藏0层和Defpoints图层
  (setq varS(ssget "A" (list
			 (cons -4 "<and")
			 (cons 8  "~Defpoints")
			 (cons 8  "~0")
			 (cons -4 "and>")
			)
           )
  )
  (if (/= varS nil)
    (progn
      (command "hideobjects" varS "")
    )
   )
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:vvvvvvattsyngriducsicon()
(setvar "CMDECHO" 0 );
;SYSVARMONITOR系统变量监视器
;(command "ATTSYNC" "n" "@毂形位公差.111133" );更新块定义
;(command "ATTSYNC" "n" "@刹车面形位公差.111133" );更新块定义
(command "ATTSYNC" "n" "横边框2(G3000)" );更新块定义
(c:ZCENTERMARKSTYLE);更新中心线标注样式
(command "grid" "l" "y" );打开网格线
(command "ucsicon" "on" );显示ucs
(command "ucsicon" "or" );ucs显示在原点
(command "-layer" "c" "211" "0" "" );将0层颜色改为"211"
(command "-layer" "c" "t" "185,70,173" "Defpoints" "" );更改Defpoints颜色
(command "-layer" "tr" "45" "Defpoints" "" );更改Defpoints透明度
(command "SNAPMODE" 0 );关闭栅格捕捉
(setvar "OSMODE" 16381 );
(setvar "AUTOSNAP" 31);
(setvar "orthomode" 0);
(setvar "BLIPMODE" 0);
(c:zdn);切换标注样式
(command "dimlayer" "尺寸线" );标注图层
(command "textstyle" "@Standard" );文字样式
(c:zt);切换图层
(command "-layer"
     "off" "参考点"
     "off" "参考线"
     "off" "临时图形"
     ""
     )
(command "regenall")
(setvar "CMDECHO" 1 );

)

(defun c:zps(/ varV)
  (setq varV(getvar "pickstyle"))
  (if (/= varV 0)
    (progn (setvar "pickstyle" 0) )
    (progn (setvar "pickstyle" 1) )
    )
)
(defun c:zgs() (c:zps) )

(defun c:zshowallobject()
  (command "layon" )
	(command "unisolateobjects" )
)

 (defun c:zhinsert(/ varS)
 	(setq varS(ssget "A" (list (cons 0 "INSERT"))))
	(if (/= varS nil )(progn
		(command "hideobjects" varS "")
	) )
 )

(defun c:zhideinsert(/ varS)(c:zhinsert))

(defun c:zbylayer(/ varS)
   (setq varS( ssget ":E" ) );select object
   (if (/= varS nil)(progn
   ;(command "change" varS "" "p" "la" "Defpoints" "" )       ;切换图层到Defpoints
   ;(command "change" varS "" "p" "c"  "bylayer" "" ) ;转换图层颜色到bylayer
   ;(command "change" varS "" "p" "lt" "bylayer" "" ) ;转换图层线型到bylayer
   ;(command "change" varS "" "p" "lw" "bylayer" "" ) ;转换图层宽度到bylayer
   (command "setbylayer" varS "" "Y" "N" )
   ));if
)

(defun c:zbyblock(/ varS)
   (setq varS( ssget ":E" ) );select object
   (if (/= varS nil)(progn
   (command "change" varS "" "p" "la" "0" "" )       ;切换图层到0
   (command "change" varS "" "p" "c"  "byblock" "" ) ;转换图层颜色到bylayer
   (command "change" varS "" "p" "lt" "byblock" "" ) ;转换图层线型到bylayer
   (command "change" varS "" "p" "lw" "byblock" "" ) ;转换图层宽度到bylayer
   ));Defpoints
)
;
(defun c:ZCENTERMARKSTYLE()
  (command "CENTEREXE" 3 )             ;中心线延伸长度
  (command "CENTERLAYER" "中心线" )    ;中心线图层
  (command "CENTERLTSCALE" 1 )         ;中心线线型比例
  (command "CENTERLTYPE" "BYLAYER" )   ;中心线线型
)

;复制
(defun c:zqc()
  (command "copyclip")
)
;黏贴
(defun c:zqv()
  (command "pasteclip")
)
;冻结所有图层
(defun c:zlockall()
  (setvar "CMDECHO" 0)
  (command "-layer" "lo" "0,中心线层,虚线层,细实线层,粗实线层,尺寸线层,剖面线层,隐藏层,Defpoints,图层2,尺寸线,粗实线,图层1,BIAOZHU,标注,细实线,中心线,虚线,不打印,BZ,双点划线,TEXT,参考线,图纸界限,临时图形,粗实线_非机械加工,粗实线_粗加工,参考点,_MarkInformation" "" )
  (setvar "CMDECHO" 1)
)
;解冻所有图层
(defun c:zunlockall()
  (setvar "CMDECHO" 0)
  (command "-layer" "u" "0,中心线层,虚线层,细实线层,粗实线层,尺寸线层,剖面线层,隐藏层,Defpoints,图层2,尺寸线,粗实线,图层1,BIAOZHU,标注,细实线,中心线,虚线,不打印,BZ,双点划线,TEXT,参考线,图纸界限,临时图形,粗实线_非机械加工,粗实线_粗加工,参考点,_MarkInformation" "" )
  (setvar "CMDECHO" 1)
)

;分解块 , 删除约束 , 修改图层到参考线 , 删除块中的点 删除Defpoints对象
(defun c:zexplode( / varSName varSObject varS varS1  varI varSName1 varSObject1 varJ varSSNameList )
  ( setq varS nil )
  ( setq varI 0 )
  (setq varS( ssget ":E" ) );select object
  (if (/= varS nil)
  (progn

	  (setvar "CMDECHO" 0)
	  (command "DELCONSTRAINT" varS "")               ;删除约束
          (command "change" varS "" "p" "la" "参考线" "") ;切换图层到参考线
          (command "change" varS "" "p" "c" "bylayer" "") ;转换图层颜色到bylayer
          (command "change" varS "" "p" "lt" "bylayer" "");转换图层线型到bylayer
          (command "change" varS "" "p" "lw" "bylayer" "");转换图层宽度到bylayer
	  (setvar "CMDECHO" 1)

      (repeat (sslength varS)
	  (setq varSName (ssname varS varI) )
	  (setq varSObject (entget varSName ))
	  (setq varI(+ varI 1) );;
	  (if (= (cdr (assoc 0 varSObject )) "INSERT" )
	  (progn
	  (command "explode" varSName  "" )
          (setq varS1(ssget "p"));选择分解后的对象
	  (if  (/= varS1 nil)
	  (progn
	  (setvar "CMDECHO" 0)
	  (setq varSSNameList () )

          ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	  ( setq varJ 0 )
	  (repeat (sslength varS1)
	    (setq varSName1 (ssname varS1 varJ) )
	    (setq varSObject1 (entget varSName1 ))
	    (setq varJ(+ varJ 1) );;
	    (if (or (= (cdr (assoc 0 varSObject1 )) "POINT" )
		    (= (cdr (assoc 8 varSObject1 )) "Defpoints" )
		)
	      (progn
	      (setq varSSNameList(append varSSNameList (list varSName1) ) )
	      (command "erase" varSName1 "" )
	      );progn
	    );if
	  )
	  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	  ( setq varJ 0 )
	  (repeat (length varSSNameList)
	    (setq  varS1(ssdel (nth varJ varSSNameList ) varS1 ) )
	    (setq varJ(+ varJ 1) );;
	  )
	  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	  (if (/= varS1 nil)
	  (progn
	  (command "DELCONSTRAINT" varS1 "")               ;删除约束
          (command "change" varS1 "" "p" "la" "参考线" "") ;切换图层到参考线
          (command "change" varS1 "" "p" "c" "bylayer" "") ;转换图层颜色到bylayer
          (command "change" varS1 "" "p" "lt" "bylayer" "");转换图层线型到bylayer
          (command "change" varS1 "" "p" "lw" "bylayer" "");转换图层宽度到bylayer
	  ))

	  (setvar "CMDECHO" 1)
	  );progn
	  );if
	  );progn
	  );if
     );repeat
  );progn
  );if
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zsd()
  (command "dcdisplay" pause )
  )
(defun c:zsg()
  (command "ConstraintBar" pause )
  )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zw()
  (command "CLEANSCREENON" )
  )

(defun c:zwo()
  (command "CLEANSCREENOFF" )
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zbreak()
  (command "break" pause "f" pause )
  )

(defun c:zpbreak()
  (command "break" pause "f" pause "@" )
  )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zbbo(/ varS)
	(setq varS(ssget "A" (list (cons 8 "粗实线"))))
	(if (/= varS nil )(progn
		(command "hideobjects" varS "")
	) )

    (command "-layer" "on" "粗实线"   "")
    (command "-layer" "off" "0"   "")
    (command "-layer" "off" "参考点"   "")
    (command "-layer" "off" "临时图形"   "")
    (command "-layer" "off" "Defpoints"   "")
    (setvar "clayer" "粗实线")

)

(defun c:zebo(/ varS)
	(command "-layer" "on" "参考点"   "")
    (command "-layer" "on" "临时图形"   "")
    (command "-layer" "on" "Defpoints"   "")
    (command "-layer" "on" "0"   "")
    (command "unisolateobjects" )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zbo( /
	*error*
	varCLayer
	varS
	)

(setq  varCLayer (getvar "clayer") )
(defun *error* ( msg )
	  	;(command "-layer" "on" "参考点" "")
        	(setvar "clayer"  varCLayer )
        	(command "unisolateobjects" )
        	(princ "Zzz:error: ")
        	(princ msg)
        	(princ "\n")
        	(princ)
	  )

	(setq varS(ssget "A" (list (cons 8 "粗实线"))))
	(if (/= varS nil )(progn
		(command "hideobjects" varS "")
	) )

  	(setvar "clayer" "粗实线")

    (command "-layer" "off" "0"   "")
    (command "-layer" "off" "参考点"   "")
    (command "-layer" "off" "临时图形"   "")
    (command "-layer" "off" "Defpoints"   "")

    (command "regen")
    (command "boundary"  pause "" )
    (setvar "clayer" "粗实线")

    (command "-layer" "on" "参考点"   "")
    (command "-layer" "on" "临时图形"   "")
    (command "-layer" "on" "Defpoints"   "")
    (command "-layer" "on" "0"   "")
    (setvar "clayer"  varCLayer )
    (command "unisolateobjects" )

)
(defun c:zx() (command "line") )
(defun c:zxx() (command "point") )
(defun c:zxc() (c:zpline) )
;;"GEOMCONSTRAINT"
;;"DIMCONSTRAINT"
(defun c:zfgh() (command "GEOMCONSTRAINT" "H" pause ) )    ;水平
(defun c:zfgv() (command "GEOMCONSTRAINT" "v" pause ) )    ;垂直
(defun c:zfgp() (command "GEOMCONSTRAINT" "P" pause ) )    ;垂直
(defun c:zfgpa() (command "GEOMCONSTRAINT" "Pa" pause ) )  ;平行
(defun c:zfgt() (command "GEOMCONSTRAINT" "t" pause ) )    ;相切
(defun c:zfgsm() (command "GEOMCONSTRAINT" "sm" pause ) )  ;平滑
(defun c:zfgc() (command "GEOMCONSTRAINT" "c" pause ) )    ;重合
(defun c:zfgcon() (command "GEOMCONSTRAINT" "con" pause ) );同心
(defun c:zfgcol() (command "GEOMCONSTRAINT" "col" pause ) );共线
(defun c:zfgs() (command "GEOMCONSTRAINT" "s" pause ) )    ;对称
(defun c:zfge() (command "GEOMCONSTRAINT" "e" pause ) )    ;相等
(defun c:zfgf() (command "GEOMCONSTRAINT" "f" pause ) )    ;固定

(defun c:gg() (command "DIMCONSTRAINT" "l" pause ) )
(defun c:gge() (command "GEOMCONSTRAINT" "e" pause ) )    ;相等
(defun c:gga() (command "DIMCONSTRAINT" "a" pause ) )
(defun c:ggaa() (command "DIMCONSTRAINT" "an" pause ) )
(defun c:ggr() (command "DIMCONSTRAINT" "r" pause ) )
(defun c:ggd() (command "DIMCONSTRAINT" "d" pause ) )
(defun c:ggt() (command "GEOMCONSTRAINT" "t" pause ) )
(defun c:ggc() (command "GEOMCONSTRAINT" "c" pause ) )
(defun c:ggf() (command "GEOMCONSTRAINT" "f" pause ) )
(defun c:ggcf() (command "GEOMCONSTRAINT" "col" pause ) )
(defun c:ggcc() (command "GEOMCONSTRAINT" "con" pause ) )

(defun c:zfdl() (command "DIMCONSTRAINT" "l" pause ) )     ;线性
(defun c:zfdh() (command "DIMCONSTRAINT" "h" pause ) )     ;水平
(defun c:zfdv() (command "DIMCONSTRAINT" "v" pause ) )     ;垂直
(defun c:zfda() (command "DIMCONSTRAINT" "a" pause ) )     ;对齐
(defun c:zgg()  (command "DIMCONSTRAINT" "a" "2l" pause ) );线间距
(defun c:zff()  (command "DIMCONSTRAINT" "a" "o"  pause ) );对象
(defun c:zfdan()(command "DIMCONSTRAINT" "an" pause ) )    ;角度
(defun c:zfdr() (command "DIMCONSTRAINT" "r" pause ) )     ;半径
(defun c:zfdd() (command "DIMCONSTRAINT" "d" pause ) )     ;直径

(defun c:zfgh水平() (command "GEOMCONSTRAINT" "H" pause ) )    ;水平
(defun c:zfgv垂直() (command "GEOMCONSTRAINT" "v" pause ) )    ;垂直
(defun c:zfgp垂直() (command "GEOMCONSTRAINT" "P" pause ) )    ;垂直
(defun c:zfgpa平行() (command "GEOMCONSTRAINT" "Pa" pause ) )  ;平行
(defun c:zfgt相切() (command "GEOMCONSTRAINT" "t" pause ) )    ;相切
(defun c:zfgsm平滑() (command "GEOMCONSTRAINT" "sm" pause ) )  ;平滑
(defun c:zfgc重合() (command "GEOMCONSTRAINT" "c" pause ) )    ;重合
(defun c:zfgcon同心() (command "GEOMCONSTRAINT" "con" pause ) );同心
(defun c:zfgcol共线() (command "GEOMCONSTRAINT" "col" pause ) );共线
(defun c:zfgs对称() (command "GEOMCONSTRAINT" "s" pause ) )    ;对称
(defun c:zfge相等() (command "GEOMCONSTRAINT" "e" pause ) )    ;相等
(defun c:zfgf固定() (command "GEOMCONSTRAINT" "f" pause ) )    ;固定

(defun c:zfdl线性() (command "DIMCONSTRAINT" "l" pause ) )     ;线性
(defun c:zfdh水平() (command "DIMCONSTRAINT" "h" pause ) )     ;水平
(defun c:zfdv垂直() (command "DIMCONSTRAINT" "v" pause ) )     ;垂直
(defun c:zfda对齐() (command "DIMCONSTRAINT" "a" pause ) )     ;对齐
(defun c:zgg线间距()  (command "DIMCONSTRAINT" "a" "2l" pause ) );线间距
(defun c:zff对象()  (command "DIMCONSTRAINT" "a" "o"  pause ) );对象
(defun c:zfdan角度()(command "DIMCONSTRAINT" "an" pause ) )    ;角度
(defun c:zfdr半径() (command "DIMCONSTRAINT" "r" pause ) )     ;半径
(defun c:zfdd直径() (command "DIMCONSTRAINT" "d" pause ) )     ;直径
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;连接直线
(defun c:zzc()
  (setvar "TRIMMODE" 1 ) ;;;
  (setvar "FILLETRAD" 0 );;;
  (command "fillet" pause );
 )
;;绘制相切圆弧
(defun c:zzf()
  (command "fillet" "r" pause )
  (setvar "TRIMMODE" 0 );
  (command "fillet"    );
)
;;绘制相切圆弧
(defun c:zzff(/ varR0 varR )
  (command "fillet" "r" pause )
  (setvar "TRIMMODE" 1 );
  (command "fillet"    );
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zmc()
  (command "centermark" pause)
  )
(defun c:zml()
   (command "centerline" pause)
  )

(defun c:zsa( / varV )
  ( setq varV (getvar "pickadd") )
  (if (/= varV 2)
    (progn (setvar "pickadd" 2) )
    (progn (setvar "pickadd" 0) )
  )
)

;;;;
;pickadd
;;;;
(defun c:zpa()
  (command "pickadd" 2 )
)

(defun c:zpaoff()
  (command "pickadd" 0 )
)

(defun get_all_layer_name(/ varL )
   ( setq varL "尺寸线,0,中心线层,虚线层,细实线层,粗实线层,尺寸线层,剖面线层,隐藏层,Defpoints,图层2,粗实线,图层1,BIAOZHU,标注,细实线,中心线,虚线,不打印,BZ,双点划线,TEXT,参考线,图纸界限,临时图形,粗实线_非机械加工,粗实线_粗加工,参考点" )
   )

;
(defun c:cc1( / varSelect varColor )
  (setq varSelect(ssget ":E" ) )
  ;(command "change" )
  (if (/= varSelect nil)(progn
  (setq varColor (getvar "cecolor") )
  (command "change" varSelect "" "p" "c" "t" "222,78,12"  "" )))
)
;
(defun c:cc2( / varSelect varColor )
  (setq varSelect(ssget ":E" ) )
  ;(command "change" )
  (if (/= varSelect nil)(progn
  (setq varColor (getvar "cecolor") )
  (command "change" varSelect "" "p" "c" "t" "142,240,56"  "" )))
)
;
(defun c:cc3( / varSelect varColor )
  (setq varSelect(ssget ":E" ) )
  ;(command "change" )
  (if (/= varSelect nil)(progn
  (setq varColor (getvar "cecolor") )
  (command "change" varSelect "" "p" "c" "t" "13,200,255"  "" )))
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zgdns();隐藏所有约束
  (command "DCDISPLAY" "all" "" "H" )
  (command "CONSTRAINTBAR" "all" "" "H" )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun ____z_my_error(S)
  ( princ "\nmy error\n")
  (princ (strcat "\n ERROR :  " S) )
  ____z_load_state()
  ;( princ "\nmy error\n")
)

(defun ____z_save_state()
  ;保存old值
  (setq ____zg_osmode (getvar "OSMODE") )
  (setq ____zg_autosnap (getvar "AUTOSNAP") )
  (setq ____zg_blipmode (getvar "BLIPMODE") )
  (setq ____zg_cmdecho (getvar "CMDECHO") )
  (setq ____zg_error *error* )
  ;存储新值
  (setvar "OSMODE" 0 )    ;;16317
  (setvar "AUTOSNAP" 0 )  ;;63
  (setvar "CMDECHO" 0)
  (setvar "BLIPMODE" 0)
  (setq *error* ____z_my_error )

)


(defun zsave()
  (setvar "OSMODE" 0 )    ;;16317
  (setvar "AUTOSNAP" 0 )  ;;63
  (setvar "CMDECHO" 0 )
  (setvar "BLIPMODE" 0 )

)

(defun c:zload()
  (setvar "OSMODE" 16317 )    ;;16317
  (setvar "AUTOSNAP" 63 )     ;;63
  (setvar "CMDECHO" 1 )
  (setvar "BLIPMODE" 0 )

)

(defun ____z_load_state()
  ( princ "\nload\n")
  (setvar "OSMODE" ____zg_osmode )    ;;16317
  (setvar "AUTOSNAP" ____zg_autosnap )  ;;63
  (setvar "CMDECHO" ____zg_cmdecho )
  (setvar "BLIPMODE" ____zg_blipmode)
  (setq *error* ____zg_error )
  ( princ "\nload\n")
)

(defun c:ztest()
  ( ____z_save_state )
  ( princ "\naAA\n")
  ( command "line" pause )
  ( ____z_load_state )
  ( princ "\nrBB\n")
)

;;;定义对称点
(defun c:_____z2p(  )

      (command "_.clayer" "参考点")
      (command "point" pause)
      (command "point" pause)
      (command "GEOMCONSTRAINT" "S" "2p" pause)
)

(defun c:z2p ( /
              ;functionStart_this
              ;normalReturn_this
              *error*
              ;functionRun_this
              ;functionClearEnvironment
              varCmdEcho
              varOSMode
              varAutoSNAP
              varSNAPMode
              varBLIPMode
              varCLayer
              )

  ;cmdecho
  ;OSMODE
  ;SNAPMODE
  ;AUTOSNAP
  ;BLIPMODE


   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )

   (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun functionRun_this( /
                     varPoint0
                     varPoint1
                     )

      (functionClearEnvironment)

      (command "_.clayer" "参考点")
      (setq varPoint0(getpoint "指定第一个点:\n") )
      (command "point" varPoint0)
      (setq varPoint1(getpoint "指定第二个点:\n") )
      (command "point" varPoint1)
      (command "GEOMCONSTRAINT" "s" "2p"  varPoint0 varPoint1 pause)

    )

    (functionRun_this)
    (normalReturn_this)

    (princ)

);;;;;;;;;;;;;;;;;

;;tk
(defun zp0()
       (list
	 (float (car _0_zzp__))
	 (float (cadr _0_zzp__))
	 ;(float (caddr __zzp__))
       )
)

(defun c:czp0()
       (setq _0_zzp__ (getpoint "指定(0)点：") )
)

(defun zp1()
       (list
	 (float (car _1_zzp__))
	 (float (cadr _1_zzp__))
	 ;(float (caddr __zzp__))
       )
)

(defun c:czp1()
       (setq _1_zzp__ (getpoint "指定(1)点：") )
)


;;;;;;;;;;;;;;;;;;;;;;

(defun c:zr()  (command "-layer" "on" "参考线" "" ) ( command "clayer" "参考线"));;;     切换到图层参考线
(defun c:zs()  (command "-layer" "on" "尺寸线" "" ) ( command "clayer" "尺寸线"));;;     切换到图层尺寸线
(defun c:zt()  (command "-layer" "on" "粗实线" "" ) ( command "clayer" "粗实线"));;;     切换到图层粗实线
(defun c:ztd() (command "-layer" "on" "双点划线" "" ) (command "clayer" "双点划线"));;;  切换到图层双点划线
(defun c:zf()  (command "-layer" "on" "细实线" "" ) ( command "clayer" "细实线"));;;     切换到图层细实线
(defun c:zd()  (command "-layer" "on" "虚线" "" ) ( command "clayer" "虚线"));;;       切换到图层虚线         dotted line
(defun c:zc()  (command "-layer" "on" "中心线" "" ) ( command "clayer" "中心线"));;;     切换到图层中心线
(defun c:zl()  (command "-layer" "on" "图纸界限" "" ) ( command "clayer" "图纸界限"));;;   切换到图层图纸界限
(defun c:zor() (command "-layer" "on" "临时图形" "" ) (command "clayer" "临时图形"));;;  切换到临时图形
(defun c:zpr() (command "-layer" "on" "参考点" "" ) (command "clayer" "参考点"));;;    切换到临时图形
(defun c:zz()  (command "-layer" "on" "0" "" ) ( command "clayer" "0") );;;          切换到0层
(defun c:ztn() (command "-layer" "on" "粗实线_非机械加工" "" ) (command "clayer" "粗实线_非机械加工"));;;  切换到粗实线_非机加工

(defun c:zpon()(command "PICKSTYLE" "1"));;;     打开组选择
(defun c:zpoff()(command "PICKSTYLE" "0"));;;    关闭组选择

(defun c:cztn( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "粗实线_非机械加工" "c" "BYLAYER" "" ))
(defun c:czr( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "参考线" "c" "BYLAYER" "" ))
(defun c:czs( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "尺寸线" "c" "BYLAYER" "" ))
(defun c:czt( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "粗实线" "c" "BYLAYER" "" ))
(defun c:cztd( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "双点划线" "c" "BYLAYER" "" ))
(defun c:czf( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "细实线" "c" "BYLAYER" "" ))
(defun c:czd( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "虚线" "c" "BYLAYER" "" ))
(defun c:czc( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "中心线" "c" "BYLAYER" "" ))
(defun c:czl( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "图纸界限" "c" "BYLAYER" "" ))
(defun c:czor( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "临时图形" "c" "BYLAYER" "" ))
(defun c:czz( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "0" "c" "BYLAYER" "" ))

(defun c:zdhal()  (command "dimstyle" "R" "一半_默认公差L") );切换标注样式为一半_默认公差L
(defun c:zdha()  (command "dimstyle" "R" "一半_默认公差") );切换标注样式为一半_默认公差
(defun c:zdn()  (command "dimstyle"  "R" "默认公差") );切换标注样式为默认公差
(defun c:zdl()  (command "dimstyle"  "R" "极限公差") );切换标注样式为极限公差
(defun c:zds()  (command "dimstyle"  "R" "对称公差") );切换标注样式为对称公差
(defun c:zdv()  (command "dimstyle"  "R" "加框_默认公差") );切换标注样式为加框_默认公差
(defun c:zdlo() (command "dimstyle"  "R" "极限公差_0P1") );切换标注样式为极限公差_0P1
(defun c:zdle() (command "dimstyle"  "R" "极限公差_0P8") );切换标注样式为极限公差_0P8
(defun c:zdlt() (command "dimstyle"  "R" "极限公差_0P3") );切换标注样式为极限公差_0P3
(defun c:zdls() (command "dimstyle"  "R" "极限公差_0P2") );切换标注样式为极限公差_0P2
(defun c:zdsf() (command "dimstyle"  "R" "对称公差_0P15") );切换标注样式为 对称公差_0P15
(defun c:zdst() (command "dimstyle"  "R" "对称公差_0P12") );切换标注样式为 对称公差_0P12

(defun c:czdhaL( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "一半_默认公差L")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdha( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "一半_默认公差")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdn( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "默认公差")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdl( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "极限公差")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czds( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "对称公差")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdv( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "加框_默认公差")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdlo( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "极限公差_0P1")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdlt( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "极限公差_0P3")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdls( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "极限公差_0P2")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdle( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "极限公差_0P8")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdsf( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "对称公差_0P15")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )
(defun c:czdst( / varS )(setq varS( ssget ":E" ))(command "dimstyle" "R" "对称公差_0P12")(command "dimstyle" "A" varS "" )(command "dimstyle" "R" "默认公差" ) )

;(getvar "cmleaderstyle")
(defun c:zmldp( /
	       *error*
	       varCLayer
	       )

  (setq  varCLayer   (getvar "clayer") )

  (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "clayer" "尺寸线" )
  (setvar "cmleaderstyle" "引线Point" )
  (command "mleader" pause)
  (normalReturn_this)

)

(defun c:zmldn( /
	       *error*
	       varCLayer
	       )

  (setq  varCLayer   (getvar "clayer") )

  (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "clayer" "尺寸线" )
  (setvar "cmleaderstyle" "引线标注" )
  (command "mleader" pause)
  (normalReturn_this)

)

(defun c:zmld( /
	       *error*
	       varCLayer
	       )

  (setq  varCLayer   (getvar "clayer") )

  (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "clayer" "尺寸线" )
  (setvar "cmleaderstyle" "无箭头引线" )
  (command "mleader" pause)
  (normalReturn_this)

)

(defun c:zmlds(/
	       *error*
	       varCLayer)

(setq  varCLayer   (getvar "clayer") )

  (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (princ "\n")

    )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "clayer" "尺寸线" )

  (setvar "cmleaderstyle" "Standard" )
  (command "mleader" pause)
  (normalReturn_this)

)

(defun div2(x)(/ x 2.0));;;                      除以2.0
(defun z2(x)(/ x 2.0));;;                        除以2.0

;;;保存
(defun c:zqs( / *error* )

(defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  ;;;将尺寸线置于顶层
(defun c_zsu( / varS )

  (command "hatchtoback" )
  (command "texttofront" "a")

)

  ;;;将粗实线置于顶层
  (defun c_ztu( / varS )

  (setq varS(ssget "A" (list (cons 8 "粗实线*"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "F" )
  )

  ;(setq varS(ssget "A" (list (cons 8 "粗实线_非机械加工"))))
  ;(if(/= varS nil)
  ;	(command "DRAWORDER" varS "" "F" )
  ;)

)

  (setvar "cmdecho" 0)

  (c_ztu)
  (c_zsu)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (command "-layer"  "p" "n" "参考线" ""  )
  (command "-layer"  "p" "n" "参考点" ""  )
  (command "-layer"  "p" "n" "临时图形" ""  )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  (command "qsave")
  (setvar "cmdecho" 1)
)

(defun c:zopen()
  (command "-layer"  "p" "p" "参考线" ""  )
  (command "-layer"  "p" "p" "参考点" ""  )
  (command "-layer"  "p" "p" "临时图形" ""  )
)

;;将尺寸线至于顶层
(defun c:zsu( / varS )

  (setq varS(ssget "A" (list (cons 8 "尺寸线"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "F" )
  )

)

;;将粗实线置为顶层
(defun c:ztu( / varS )

  (setq varS(ssget "A" (list (cons 8 "粗实线"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "F" )
  )

  (setq varS(ssget "A" (list (cons 8 "粗实线_非机械加工"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "F" )
  )

)

;;将粗实线置为顶层
(defun c:dztu( / varS )

  (setq varS(ssget "A" (list (cons 8 "粗实线"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "B" )
  )

  (setq varS(ssget "A" (list (cons 8 "粗实线_非机械加工"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "B" )
  )

)

;;将临时图形置为顶层
(defun c:zoru( / varS )
  (setq varS(ssget "A" (list (cons 8 "临时图形"))))
  (if(/= varS nil)
  	(command "DRAWORDER" varS "" "F" )
  )
)

;;将参考线置为底层
;(defun c:zrd( / varS )
;  (setq varS(ssget "A" (list (cons 8 "参考线"))))
;  (if(/= varS nil)
;  	(command "DRAWORDER" varS "" "b" )
;  )
;)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;计算图幅大小
;保证 （ 宽 / 长 ） = 1.414
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(;;;输入长，宽
 defun pagesize(x y / fx fy fz);;;pagesize
  (setq fx(float x))
  (setq fy(float y))
  (setq fz(/ fx fy))
  (if (<= fz 1.414)
    (progn
      (princ "!w:")
      (princ (* fy 1.414))
      (princ ",h:")
      (princ fy)
    );;;if
    (progn
      (princ "w:")
      (princ fx)
      (princ ",!h:")
      (princ (/ fx 1.414))
    );;;else
  );;;pass if
  (princ)
  ;;;pass pagesize
)


;|concentric circles|;
;画同心圆
(defun privatezcc (/
	      point_0			;圆心|;
	      point_temp		;用户输入点|;
	      point_temp_1		;以point_0为圆心(point_temp_1,point_temp)为直径的圆|;
	     )
  (setq point_0 (getpoint "设置圆心:"))
  (if (/= point_0 nil)
    (progn
      (setq point_temp '(1,1,1))
      (while (/= point_temp nil)
	(progn
	  (setq point_temp (getpoint "圆上一点:"))
	  (if (/= point_temp nil)
	    (progn
	      (setq point_temp_1
		     (list
		       (- (* 2.0 (car point_0))
			  (car point_temp)
		       )
		       (- (* 2.0 (cadr point_0))
			  (cadr point_temp)
		       )
		       (- (* 2.0 (caddr point_0))
			  (caddr point_temp)
		       )
		     )
	      )
	      (command "circle" "2p" point_temp_1 point_temp)
	    )
	  )
	)
      )
    )
  )
  (princ)
)

(
defun c:zcc()  
 (command "arxzcc")
;  (if (/= nil arxzcc ) 
;  (progn
;    (command "arxzcc")
;  )
;  (progn
;    (privatezcc)
;  )
; )
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;draw bound
(defun c:zdb ( /
   argWidth
	 argHeight
	 argPointTopLeft
	 varRate
   varCmdEcho
   varOSMode
   varAutoSNAP
   varSNAPMode
   varBLIPMode
   *error*
   varCLayer
	      )

   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )

   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (defun zDrawBoundInner (varPointTopLeft	      varWidth
			  varHeight	/	      varIndex
			  varOffset	varP0	      varP1
			  ABCDEFlist
			 )

;;;辅助函数
    (defun pointAdd (a b)
      (list (+ (float (car a)) (float (car b)))
	    (+ (float (cadr a)) (float (cadr b)))
	    (+ (float (caddr a)) (float (caddr b)))
      )
    )

;;;绘制界限文字
    (defun functionDrawString (beginPoint endPoint drawString)
      (command "_.mtext"  beginPoint "S"	"界限ABCDEF"
	       "J"	  "mc"	     "C"	"n"	   endPoint
	       drawString ""
	      )
      (princ)
    )
;;;设置图层
    (command "_.clayer" "图纸界限")
;;;绘制外边框
    (defun drawOuterBound (varPTL varW varH / varP1 varP2 varP3)
      (setq varP1 (pointAdd varPTL (list varW 0.0 0.0)))
      (setq varP2 (pointAdd varPTL (list varW (- 0.0 varH) 0.0)))
      (setq varP3 (pointAdd varPTL (list 0.0 (- 0.0 varH) 0.0)))
      (command "_.line" varPTL varP1 varP2 varP3 "C")
      (princ "绘制外边框完成:")
      (princ "\r")
    )
    (drawOuterBound varPointTopLeft varWidth varHeight)
;;;设置图层
    (command "_.clayer" "粗实线")
;;;设置内边框
    (defun drawInnerBound (varPTL1 varW1 varH1 / varP11 varP21 varP31)
      (setq varP11 (pointAdd varPTL1 (list varW1 0.0 0.0)))
      (setq varP21 (pointAdd varPTL1 (list varW1 (- 0.0 varH1) 0.0)))
      (setq varP31 (pointAdd varPTL1 (list 0.0 (- 0.0 varH1) 0.0)))
      (command "_.line" varPTL1 varP11 varP21 varP31 "C")
      (princ "绘制内边框完成:")
      (princ "\r")
    )
    (drawInnerBound
      (pointAdd varPointTopLeft (list 20.0 -20.0 0.0))
      (- varWidth 40.0)
      (- varHeight 40.0)
    )
;;;计算偏移量
    (setq varOffset (/ (- varHeight 40.0) 6.0))
;;;设置图层
    (command "_.clayer" "图纸界限")
;;;绘制文字边框线
    (setq varIndex 0)
    (setq varP0 (pointAdd varPointTopLeft (list 0.0 -20.0 0.0)))
    (setq varP1	(pointAdd varPointTopLeft
			  (list (- varWidth 20.0) -20.0 0.0)
		)
    )
    (repeat 5
      (progn
	(setq varIndex (- varIndex 1))
	(command "_.line"
		 (pointAdd varP0 (list 0 (* varIndex varOffset) 0))
		 "@20,0"
		 ""
	)
	(command "_.line"
		 (pointAdd varP1 (list 0 (* varIndex varOffset) 0))
		 "@20,0"
		 ""
	)
      )
    )
;;;

;;;设置图层
    (command "_.clayer" "图纸界限")
;;;计算偏移量
    (setq varIndex 1)
    (setq ABCDEFlist (list "?" "A" "B" "C" "D" "E" "F" "!"))
    (setq varOffset (* -1.0 varOffset))
    (setq
      varP0 (pointAdd varPointTopLeft
		      (list (- varWidth 20.0) (- -20.0 varOffset) 0.0)
	    )
    )
    (setq varP1	(pointAdd varPointTopLeft
			  (list varWidth -20.0 0.0)
		)
    )
;;;绘制文字
    (repeat 6
      (progn
	(functionDrawString
	  (pointAdd varP0 (list 0 (* varIndex varOffset) 0))
	  (pointAdd varP1 (list 0 (* varIndex varOffset) 0))
	  (nth varIndex ABCDEFlist)
	)
	(setq varIndex (+ 1 varIndex))
      )
    )

  )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;获得数据
  (setq argPointTopLeft (getpoint "获得左上角的点:"))
  (setq argWidth (getdist "输入矩形的宽:"))
  (setq argHeight (getdist "输入矩形的长:"))

  (setq argWidth (+ 40.0 argWidth))
  (setq argHeight (+ 40.0 argHeight))

;;;计算图幅
  (setq varRate (/ (float argWidth) (float argHeight)))

					;OSMODE对象捕捉
					;AUTOSNAP追踪
  (functionClearEnvironment)

  (if (< varRate 1.414)
    (zDrawBoundInner
      argPointTopLeft
      (* argHeight 1.414)
      argHeight
    )
    (zDrawBoundInner
      argPointTopLeft
      argWidth
      (/ argWidth 1.414)
    )
  )

  (normalReturn_this)

  (princ)

);end of zdb

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;draw bound
(defun c:zdb1 ( /
   argWidth
   argHeight
   argPointTopLeft
   varRate
   varCmdEcho
   varOSMode
   varAutoSNAP
   varSNAPMode
   varBLIPMode
   *error*
   varCLayer
        )

   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )

   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (defun zDrawBoundInner (varPointTopLeft
        varWidth
        varHeight  /
        varIndex
        varOffset
        varP0
        varP1
        ABCDEFlist
       )

;;;辅助函数
    (defun pointAdd (a b)
      (list (+ (float (car a)) (float (car b)))
      (+ (float (cadr a)) (float (cadr b)))
      (+ (float (caddr a)) (float (caddr b)))
      )
    )

;;;绘制界限文字
    (defun functionDrawString (beginPoint endPoint drawString)
      (command "_.mtext"  beginPoint "S"  "界限ABCDEF"
         "J"    "mc"       "C"  "n"    endPoint
         drawString ""
        )
      (princ)
    )
;;;设置图层
    (command "_.clayer" "图纸界限")
;;;绘制外边框
    (defun drawOuterBound (varPTL varW varH / varP1 varP2 varP3)
      (setq varP1 (pointAdd varPTL (list varW 0.0 0.0)))
      (setq varP2 (pointAdd varPTL (list varW (- 0.0 varH) 0.0)))
      (setq varP3 (pointAdd varPTL (list 0.0 (- 0.0 varH) 0.0)))
      (command "_.line" varPTL varP1 varP2 varP3 "C")
      (princ "绘制外边框完成:")
      (princ "\r")
    )
    (drawOuterBound varPointTopLeft varWidth varHeight)
;;;设置图层
    (command "_.clayer" "粗实线")
;;;设置内边框
    (defun drawInnerBound (varPTL1 varW1 varH1 / varP11 varP21 varP31)
      (setq varP11 (pointAdd varPTL1 (list varW1 0.0 0.0)))
      (setq varP21 (pointAdd varPTL1 (list varW1 (- 0.0 varH1) 0.0)))
      (setq varP31 (pointAdd varPTL1 (list 0.0 (- 0.0 varH1) 0.0)))
      (command "_.line" varPTL1 varP11 varP21 varP31 "C")
      (princ "绘制内边框完成:")
      (princ "\r")
    )
    (drawInnerBound
      (pointAdd varPointTopLeft (list 20.0 -20.0 0.0))
      (- varWidth 40.0)
      (- varHeight 40.0)
    )
;;;计算偏移量
    (setq varOffset (/ (- varHeight 40.0) 6.0))
;;;设置图层
    (command "_.clayer" "图纸界限")
;;;绘制文字边框线
    (setq varIndex 0)
    (setq varP0 (pointAdd varPointTopLeft (list 0.0 -20.0 0.0)))
    (setq varP1 (pointAdd varPointTopLeft
        (list (- varWidth 20.0) -20.0 0.0)
    )
    )
    (repeat 5
      (progn
  (setq varIndex (- varIndex 1))
  (command "_.line"
     (pointAdd varP0 (list 0 (* varIndex varOffset) 0))
     "@20,0"
     ""
  )
  (command "_.line"
     (pointAdd varP1 (list 0 (* varIndex varOffset) 0))
     "@20,0"
     ""
  )
      )
    )
;;;

;;;设置图层
    (command "_.clayer" "图纸界限")
;;;计算偏移量
    (setq varIndex 1)
    (setq ABCDEFlist (list "?" "A" "B" "C" "D" "E" "F" "!"))
    (setq varOffset (* -1.0 varOffset))
    (setq
      varP0 (pointAdd varPointTopLeft
          (list (- varWidth 20.0) (- -20.0 varOffset) 0.0)
      )
    )
    (setq varP1 (pointAdd varPointTopLeft
        (list varWidth -20.0 0.0)
    )
    )
;;;绘制文字
    (repeat 6
      (progn
  (functionDrawString
    (pointAdd varP0 (list 0 (* varIndex varOffset) 0))
    (pointAdd varP1 (list 0 (* varIndex varOffset) 0))
    (nth varIndex ABCDEFlist)
  )
  (setq varIndex (+ 1 varIndex))
      )
    )

  )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;获得数据
  (setq argPointTopLeft (getpoint "获得左上角的点:"))
  (setq argWidth (getdist "输入矩形的宽:"))
  (setq argHeight (getdist "输入矩形的长:"))

  (setq argWidth (+ 40.0 argWidth))
  (setq argHeight (+ 40.0 argHeight))

;;;计算图幅
  (setq varRate (/ (float argHeight) (float argWidth)  ) )

          ;OSMODE对象捕捉
          ;AUTOSNAP追踪
  (functionClearEnvironment)

  (if (> varRate 1.414 )
    (zDrawBoundInner
      argPointTopLeft
      (/ argHeight 1.414 );width
      argHeight           ;height
    )
    (zDrawBoundInner
      argPointTopLeft
      argWidth            ;width
      (* argWidth 1.414 )  ;height
    )
  )

  (normalReturn_this)

  (princ)

);end of zdb1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;drawRoundRect
;;zdrr
(defun c:zdrr(/
		     varPointStart
		     varWidth
		     varHeight
		     varR
		     varRI
		     varPointC1
		     varPointC2
         varCmdEcho
         varOSMode
         varAutoSNAP
         varSNAPMode
         varBLIPMode
         *error*
         varCLayer
		     )

(defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )

   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

(setq varPointStart(getpoint "起点：" ))
(setq varWidth(getdist "宽："))
(setq varHeight(getdist "高："))

  (defun pointAdd (a b)
      (list (+ (float (car a)) (float (car b)))
	    (+ (float (cadr a)) (float (cadr b)))
	    (+ (float (caddr a)) (float (caddr b)))
      )
    )

  (setq varR (/ varWidth 2.0))
  (setq varRI(/ varWidth -2.0))
  (setq varPointC1(pointAdd varPointStart (list 0.0 varRI 0.0)))
  (setq varPointC2(pointAdd varPointStart (list 0.0 (- varR varHeight) 0.0)))


  (functionClearEnvironment)
  ;;;绘制矩形

   (command "arc"
	   varPointStart
	   "C" varPointC1
	   "A" 90
   )

  (command "line"
	   (pointAdd varPointC1 (list varRI 0 0))
	   (pointAdd varPointC2 (list varRI 0 0))
	   ""
   )

  (command "arc"
	   (pointAdd varPointC2 (list varRI 0 0))
	   "C" varPointC2
	   "A" 90
  )

  (command "arc"
	   (pointAdd varPointC2 (list 0 varRI 0))
	   "C" varPointC2
	   "A" 90
  )

   (command "line"
	   (pointAdd varPointC1 (list varR 0 0))
	   (pointAdd varPointC2 (list varR 0 0))
	   ""
   )


  (command "arc"
	   (pointAdd varPointC1 (list varR 0 0))
	   "C" varPointC1
	   "A" 90
  )

  (normalReturn_this)

)
;;;;;;;;;;;;;;;;;;;;;;

(defun c:zru( / varSelect )
	(setq varSelect( ssget "A" (list (cons -4   "<or") (cons 8 "参考点") (cons 8 "参考线")  (cons 8 "临时图形") (cons -4 "or>") )  ) )
	( if(/= nil varSelect )
	(command "DRAWORDER" varSelect "" "F" )
	)
)

(defun c:zrd( / varSelect )
	(setq varSelect( ssget "A" (list (cons -4   "<or") (cons 8 "参考点") (cons 8 "参考线")  (cons 8 "临时图形") (cons -4 "or>") )  ) )
	( if(/= nil varSelect )
	(command "DRAWORDER" varSelect "" "B" )
	)
)

;;;;;;;;;;;;;;;;;;;;;;
(defun c:zhlr ( /
              ;functionStart_this
              ;normalReturn_this
              *error*
              ;functionRun_this
              ;functionClearEnvironment
              varCmdEcho
              varOSMode
              varAutoSNAP
              varSNAPMode
              varBLIPMode
              varCLayer
	            varPoint0
              )

  ;cmdecho
  ;OSMODE
  ;SNAPMODE
  ;AUTOSNAP
  ;BLIPMODE

   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )

   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (defun functionRun_this( /
	             varPoint2
               varPoint1
	             ;varSelect
                     )


      (c:zru);;;;
      ;(setq varSelect nil)
      (command "_.clayer" "参考点")
      (setq varPoint0(getpoint "指定第一个点:\n") )

      (functionClearEnvironment)
      (setq varPoint1 (list (+(car varPoint0 ) 5.351) (cadr varPoint0 ) (caddr varPoint0 ) ) )
      (setq varPoint2 (list (-(car varPoint0 ) 5.351) (cadr varPoint0 ) (caddr varPoint0 ) ) )

      ;(command "point" varPoint2)
      ;(setq varSelect (ssget "L") )

      ;(command "point" varPoint1)
      ;(ssadd (ssname (ssget "L") 0) varSelect )


      (command "_.clayer" "参考线" )
      (command "line" varPoint2 varPoint1 "" )

      ;(setq varSelect (ssname (ssget "L") 0)  )
      ;(command "select" varSelect "" )
      ;(command "AUTOCONSTRAIN" varSelect  ""  )

      (command "GEOMCONSTRAINT" "H" varPoint0 )

    )

    (functionRun_this)
    (normalReturn_this)
    (command "DIMCONSTRAINT" "V" varPoint0 pause)

    (princ)

);;;;;;;;;;;;;;;;;


(defun c:zvlr ( /
              ;functionStart_this
              ;normalReturn_this
              *error*
              ;functionRun_this
              ;functionClearEnvironment
              varCmdEcho
              varOSMode
              varAutoSNAP
              varSNAPMode
              varBLIPMode
              varCLayer
	      varPoint0
              )

  ;cmdecho
  ;OSMODE
  ;SNAPMODE
  ;AUTOSNAP
  ;BLIPMODE


   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )

   (functionStart_this)

   (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )



   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     (setvar "SNAPMODE"  0 )
     (setvar "AUTOSNAP"  0 )
     (setvar "osmode"    0 )
     (setvar "BLIPMODE"  0 )

   )

    (defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (defun functionRun_this( /
	             varPoint2
               varPoint1
	             ;varSelect
	             ;varLineObj
                     )
      (c:zru);;;;
      (setq varSelect nil)
      (command "_.clayer" "参考点")
      (setq varPoint0(getpoint "指定第一个点:\n") )


      (functionClearEnvironment)
      (setq varPoint1 (list (car varPoint0 )  (+ (cadr varPoint0 ) 5.351 ) (caddr varPoint0 ) ) )
      (setq varPoint2 (list (car varPoint0 )  (- (cadr varPoint0 ) 5.351 ) (caddr varPoint0 ) ) )


      ;(command "point" (list (car varPoint0 )  (+ (cadr varPoint0 ) 5.0 ) (caddr varPoint0 ) ) )
      ;(setq varSelect (ssget "L") )

      ;(command "point" (list (car varPoint0 )  (- (cadr varPoint0 ) 5.0 ) (caddr varPoint0 ) ) )
      ;(ssadd (ssname (ssget "L") 0) varSelect )


      (command "_.clayer" "参考线" )
      (command "line" varPoint2 varPoint1 "" )
      ;(setq varLineObj (ssget "L") )
      ;(setq varSelect (ssname varLineObj 0)  )

      ;(command "select" varSelect "" )
      ;(command "AUTOCONSTRAIN" varSelect  ""  )

      (command "GEOMCONSTRAINT" "V" varPoint0 )
    )

    (functionRun_this)
    (normalReturn_this)
    (command "DIMCONSTRAINT" "H" varPoint0 pause)

    (princ)

);;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;

(defun c:zron( / *error*  )

(defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (command "-layer"
	   "on" "参考点"
	   "on" "参考线"
	   "on" "临时图形"
	   ""
	   )
  (setvar "cmdecho" 1)

  )

(defun c:zroff( / *error* )

(defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (c:zs)
  (command "-layer"
	   "off" "参考点"
	   "off" "参考线"
	   "off" "临时图形"
	   ""
	   )
  (setvar "cmdecho" 1)

  )

(defun c:zpline()
  ;PLINEGEN 设置如何围绕二维多段线的顶点生成线型图案
　;PLINETYPE 指定 AutoCAD 是否使用优化的二维多段线
　;PLINEWID 存储多段线的缺省宽度
  (command "PLINEWID" 0)
  (command "pline" pause "w" 0 0 "h" 0 0 pause )
  )

(defun c:zton( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (setvar "cmdecho" 0)
    (command "-layer" "on" "粗实线*" "" )
    (setvar "cmdecho" 1)
)

(defun c:ztoff( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (setvar "cmdecho" 0)
    (c:zor)
    (command "-layer" "off" "粗实线*" "" )
    (setvar "cmdecho" 1)
)

(defun c:zson( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (setvar "cmdecho" 0)
    (command "-layer" "on" "尺寸线" "" )
    (setvar "cmdecho" 1)
)

(defun c:zsoff( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

    (setvar "cmdecho" 0)
    (c:zor)
    (command "-layer" "off" "尺寸线" "" )
    (setvar "cmdecho" 1)
)

(defun c:zronly( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (c:zor)
    (command "-layer"
"off" "_MarkInformation"
"off" "0"
"off" "中心线层"
"off" "虚线层"
"off" "细实线层"
"off" "粗实线层"
"off" "尺寸线层"
"off" "剖面线层"
"off" "隐藏层"
"off" "Defpoints"
"off" "图层2"
"off" "粗实线"
"off" "图层1"
"off" "BIAOZHU"
"off" "标注"
"off" "细实线"
"off" "中心线"
"off" "虚线"
"off" "不打印"
"off" "BZ"
"off" "双点划线"
"off" "TEXT"
"on" "参考线"
"off" "图纸界限"
"on" "临时图形"
"off" "粗实线_非机械加工"
"off" "粗实线_粗加工"
"on" "参考点"
"off" "尺寸线"
""
  )
  (setvar "clayer" "参考线")
  (setvar "cmdecho" 1)
)

(defun c:ztonly( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (c:zt)
    (command "-layer"
"off" "_MarkInformation"
"off" "0"
"off" "中心线层"
"off" "虚线层"
"off" "细实线层"
"off" "粗实线层"
"off" "尺寸线层"
"off" "剖面线层"
"off" "隐藏层"
"off" "Defpoints"
"off" "图层2"
"on" "粗实线"
"off" "图层1"
"off" "BIAOZHU"
"off" "标注"
"off" "细实线"
"off" "中心线"
"off" "虚线"
"off" "不打印"
"off" "BZ"
"off" "双点划线"
"off" "TEXT"
"off" "参考线"
"off" "图纸界限"
"off" "临时图形"
"on" "粗实线_非机械加工"
"on" "粗实线_粗加工"
"off" "参考点"
"off" "尺寸线"
""
  )

  (setvar "cmdecho" 1)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun c:zpoint(/
	*error*
		varCLayer
		varAutoSNAP
		varSNAPMode
		varCmdEcho
		varOSMode
		varBLIPMode
	 )

   (defun functionStart_this()
     (setq  varCLayer   (getvar "clayer") )
     (setq  varAutoSNAP (getvar "AUTOSNAP") )
     (setq  varSNAPMode (getvar "SNAPMODE") )
     (setq  varCmdEcho  (getvar "cmdecho") )
     (setq  varOSMode   (getvar "osmode") )
     (setq  varBLIPMode (getvar "BLIPMODE") )
     (princ "\n")

   )
  (functionStart_this)

  (defun normalReturn_this()

     (princ "normal return")
     (setvar "clayer"   varCLayer )
     (setvar "BLIPMODE" varBLIPMode )
     (setvar "cmdecho"  varCmdEcho )
     (setvar "osmode"   varOSMode )
     (setvar "SNAPMODE" varSNAPMode )
     (setvar "AUTOSNAP" varAutoSNAP )
     (princ "\n")

    )



   (defun functionClearEnvironment()

     (setvar "cmdecho"   0 )
     ;(setvar "SNAPMODE"  0 )
     ;(setvar "AUTOSNAP"  0 )
     ;(setvar "osmode"    0 )
     ;(setvar "BLIPMODE"  0 )

   )

(defun *error* ( msg )
        (normalReturn_this)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (c:zpr)
  (command "point" pause  )
  (functionClearEnvironment)
  (normalReturn_this)
  ;(command "GEOMCONSTRAINT" "F" pause )

  )
;;;;;;;;;;;;;;;;;;;;;;

(defun c:zhonly( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (c:zf)
    (command "-layer"
"off" "_MarkInformation"
"off" "0"
"off" "中心线层"
"off" "虚线层"
"off" "细实线层"
"off" "粗实线层"
"off" "尺寸线层"
"off" "剖面线层"
"off" "隐藏层"
"off" "Defpoints"
"off" "图层2"
"on" "粗实线"
"off" "图层1"
"off" "BIAOZHU"
"off" "标注"
"on" "细实线"
"off" "中心线"
"off" "虚线"
"off" "不打印"
"off" "BZ"
"off" "双点划线"
"off" "TEXT"
"off" "参考线"
"off" "图纸界限"
"off" "临时图形"
"on" "粗实线_非机械加工"
"on" "粗实线_粗加工"
"off" "参考点"
"off" "尺寸线"
""
  )

  (setvar "cmdecho" 1)
)

(defun c:zzonly( / *error* )

    (defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)

  (c:zz)
    (command "-layer"
"off" "_MarkInformation"
"on" "0"
"off" "中心线层"
"off" "虚线层"
"off" "细实线层"
"off" "粗实线层"
"off" "尺寸线层"
"off" "剖面线层"
"off" "隐藏层"
"off" "Defpoints"
"off" "图层2"
"off" "粗实线"
"off" "图层1"
"off" "BIAOZHU"
"off" "标注"
"off" "细实线"
"off" "中心线"
"off" "虚线"
"off" "不打印"
"off" "BZ"
"off" "双点划线"
"off" "TEXT"
"off" "参考线"
"off" "图纸界限"
"off" "临时图形"
"off" "粗实线_非机械加工"
"off" "粗实线_粗加工"
"off" "参考点"
"off" "尺寸线"
""
  )

  (setvar "cmdecho" 1)
)
;;;;;;;;;;;;;;;;;;;;;;
(defun c:zdpoff()
     ( command "-layer" "off" "Defpoints" "" )
)

(defun c:zdpon()
     ( command "-layer" "on" "Defpoints" "" )
)

(defun c:zdp()
	 ( command "-layer" "on" "Defpoints" "" )
	 ( command "clayer" "Defpoints" )
)

(defun c:czdp( / varS )(setq varS( ssget ":E" ))(command "change" varS "" "p" "la" "Defpoints" "c" "BYLAYER" "" ))

;;;;;;;;;;;;;;;;;;;;;;

(defun c:zuron( / *error*  )

(defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (command "-layer"
	   "on" "参考点"
	   "on" "参考线"
	   "on" "临时图形"
	   "on" "Defpoints"
	   ""
	   )
  (setvar "cmdecho" 1)

  )

(defun c:zuroff( / *error* )

(defun *error* ( msg )
        (setvar "cmdecho" 1)
        (princ "Zzz:error: ")
        (princ msg)
        (princ "\n")
        (princ)
    )

  (setvar "cmdecho" 0)
  (c:zs)
  (command "-layer"
	   "off" "参考点"
	   "off" "参考线"
	   "off" "临时图形"
	   "off" "Defpoints"
	   ""
	   )
  (setvar "cmdecho" 1)

  )

;;;;;;;;;;;;;;;;;;;;;;
;
;
;endl of the file
;
;
;;;;;;;;;;;;;;;;;;;;;;
;mtp
;join




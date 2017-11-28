*use qmake convert pro to visual studio project
*just use ascii on paths
*open vs2017 x64 cmd

cd /D C:/autocad_tools
set PATH=C:/Qt/Qt5.9.2/5.9.2/msvc2017_64/bin;%PATH%
qmake -r -tp vc "D:/test1/HellowWord/objectarx.pro"



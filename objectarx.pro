#Visual Studio 2017 With Autocad 2018 Under Windows
#Others Will Not Be Supported

#the arx name
TARGET = _ARX_DUTY_BY_NGZ_

#add PRECOMPILED_HEADER
PRECOMPILED_HEADER  += Final.object_arx_global.hpp

#change output dir
#DESTDIR =

#the sdk include path
INCLUDEPATH += "C:/ObjectARX 2018/inc"
INCLUDEPATH += "C:/ObjectARX 2018/inc-x64"

#rxapi.lib; acdb21.lib; acge21.lib; acad.lib; ac1st21.lib; accore.lib;
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lrxapi
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacdb22
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacge22
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacad
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lac1st22
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -laccore

#DEF_FILE
DEF_FILE += $$PWD/AcRxDefault.def

##########################################################################
#cplusplus version
#CONFIG += c++17
QMAKE_CXXFLAGS += /std:c++latest

#use md dll
QMAKE_CXXFLAGS += /MD

#change .dll to .arx
TARGET_EXT = .arx

#remove _DEBUG
DEFINES -= _DEBUG

#just use Qt.Core and Qt.Gui Qt.Widget
QT       += core
QT       += gui
QT       += widgets
#CONFIG += suppress_vcproj_warnings

#Qt project template
TEMPLATE = lib
#
DEFINES += LARX_AUTOCAD_LIBRARY
#
DEFINES += QT_DEPRECATED_WARNINGS

##########################################################################

#.cpp source
SOURCES += \
        $$PWD/HellowWorld/HellowWorld.cpp \
        $$PWD/SimpleDrawLine/SimpleDrawLine.cpp \
        $$PWD/SimpleDrawLineChangeColor/SimpleDrawLineChangeColor.cpp \
        $$PWD/SimpleDrawPolygon/SimpleDrawPolygon.cpp \
        $$PWD/GetAllLayerName/GetAllLayerName.cpp \
        $$PWD/UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.cpp \
        $$PWD/TestAcDocManager/TestAcDocManager.cpp \
        $$PWD/TestCode/TestCode.cpp \
        $$PWD/UpdateLayer/UpdateLayer.cpp \
        $$PWD/UpdateDimStyle/UpdateDimStyle.cpp \
        $$PWD/UpdateTextStyle/UpdateTextStyle.cpp \
        $$PWD/UpdateReactor/UpdateReactor.cpp \
        $$PWD/AddMaskToMleader/AddMaskToMleader.cpp \
        $$PWD/UpdateMLeaderStyle/UpdateMLeaderStyle.cpp \
        $$PWD/UpdateAll/UpdateAll.cpp \
        $$PWD/UpdatePart/UpdatePart.cpp \
        $$PWD/AddMaskToMText/AddMaskToMText.cpp \
        $$PWD/UpdateTitleBlockTime/UpdateTitleBlockTime.cpp \
        $$PWD/HideDimEdge/HideDimEdge.cpp


#.h .hpp source
HEADERS += \
        $$PWD/HellowWorld/HellowWorld.hpp \
        $$PWD/SimpleDrawLine/SimpleDrawLine.hpp \
        $$PWD/SimpleDrawLineChangeColor/SimpleDrawLineChangeColor.hpp \
        $$PWD/SimpleDrawPolygon/SimpleDrawPolygon.hpp \
        $$PWD/GetAllLayerName/GetAllLayerName.hpp \
        $$PWD/UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.hpp \
        $$PWD/TestAcDocManager/TestAcDocManager.hpp \
        $$PWD/TestCode/TestCode.hpp \
        $$PWD/UpdateLayer/UpdateLayer.hpp \
        $$PWD/UpdateDimStyle/UpdateDimStyle.hpp \
        $$PWD/UpdateTextStyle/UpdateTextStyle.hpp \
        $$PWD/UpdateReactor/UpdateReactor.hpp \
        $$PWD/AddMaskToMleader/AddMaskToMleader.hpp \
        $$PWD/UpdateMLeaderStyle/UpdateMLeaderStyle.hpp \
        $$PWD/UpdateAll/UpdateAll.hpp \
        $$PWD/UpdatePart/UpdatePart.hpp \
        $$PWD/AddMaskToMText/AddMaskToMText.hpp \
        $$PWD/UpdateTitleBlockTime/UpdateTitleBlockTime.hpp \
        $$PWD/HideDimEdge/HideDimEdge.hpp


SOURCES += $$PWD/object_arx_global.cpp \
    $$PWD/ARXApplication.cpp \
    $$PWD/QtApplication.cpp

HEADERS += $$PWD/object_arx_global.hpp \
    $$PWD/ARXApplication.hpp \
    $$PWD/ArxClosePointer.hpp \
    $$PWD/AddToModelSpace.hpp \
    $$PWD/Final.object_arx_global.hpp \
    $$PWD/StringUtility.hpp

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

##########################
#                        #
#                        #
##########################


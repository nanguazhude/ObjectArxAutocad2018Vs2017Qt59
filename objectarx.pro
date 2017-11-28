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
        $$PWD/HellowWorld.cpp \
        $$PWD/SimpleDrawLine.cpp \
        $$PWD/SimpleDrawLineChangeColor.cpp \
        $$PWD/SimpleDrawPolygon.cpp \
        $$PWD/QtApplication.cpp

#.h .hpp source
HEADERS += \
        $$PWD/HellowWorld.hpp \
        $$PWD/SimpleDrawLine.hpp \
        $$PWD/SimpleDrawLineChangeColor.hpp \
        $$PWD/Final.object_arx_global.hpp \
        $$PWD/SimpleDrawPolygon.hpp

SOURCES += $$PWD/object_arx_global.cpp \
    $$PWD/ARXApplication.cpp

HEADERS += $$PWD/object_arx_global.hpp \
    $$PWD/ARXApplication.hpp \
    $$PWD/ArxClosePointer.hpp \
    $$PWD/AddToModelSpace.hpp

##########################
#                        #
#                        #
##########################


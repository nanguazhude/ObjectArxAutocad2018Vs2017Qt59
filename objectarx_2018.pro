#Visual Studio 2017 With Autocad 2018 Under Windows
#Others Will Not Be Supported

#the arx name
TARGET = _ARX_DUTY_BY_NGZ_

#add PRECOMPILED_HEADER
PRECOMPILED_HEADER  += Final.object_arx_global.hpp

#change output dir
DESTDIR = $$PWD/release/Autocad2018

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
CONFIG += suppress_vcproj_warnings
QMAKE_CXXFLAGS += /std:c++latest

#use md dll
QMAKE_CXXFLAGS += /MD

#change .dll to .arx
TARGET_EXT = .arx

#remove _DEBUG
DEFINES -= _DEBUG
DEFINES += _OBJECT_ARX_VERSION_X64_=2018

#just use Qt.Core and Qt.Gui Qt.Widget
QT       += core
QT       += gui
QT       += widgets
QT       += qml

#Qt project template
TEMPLATE = lib
#
DEFINES += LARX_AUTOCAD_LIBRARY
#
DEFINES += QT_DEPRECATED_WARNINGS

##########################################################################

include(source.pri)

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


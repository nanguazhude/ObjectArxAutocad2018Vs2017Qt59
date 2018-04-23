#Visual Studio 2017 With Autocad 2019 Under Windows
#Others Will Not Be Supported

#the arx name
TARGET = _ARX_DUTY_BY_NGZ_

CONFIG(debug,debug|release){
#add PRECOMPILED_HEADER
PRECOMPILED_HEADER  += Final.object_arx_global.hpp
}else{
#add PRECOMPILED_HEADER
PRECOMPILED_HEADER  += Final.object_arx_global.hpp
}

#change output dir
DESTDIR = $$PWD/release/Autocad2019

#the sdk include path
INCLUDEPATH += "C:/ObjectARX 2019/inc"
INCLUDEPATH += "C:/ObjectARX 2019/inc-x64"

#rxapi.lib; acdb21.lib; acge21.lib; acad.lib; ac1st21.lib; accore.lib;
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -lrxapi
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -lacdb23
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -lacge23
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -lacad
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -lac1st23
LIBS+= -L"C:/ObjectARX 2019/lib-x64" -laccore

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
DEFINES += NDEBUG
DEFINES += _OBJECT_ARX_VERSION_X64_=2019

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

include($$PWD/source.pri)
include($$PWD/ThirdPart/part_google_v8/double_conversion.pri)

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


#the arx name
TARGET = _ARX_DUTY_BY_NGZ_

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

#just use Qt.Core and Qt.Gui
QT       += core
QT       += gui
#CONFIG += suppress_vcproj_warnings

#Qt project template
TEMPLATE = lib
#
DEFINES += LARX_AUTOCAD_LIBRARY
#
DEFINES += QT_DEPRECATED_WARNINGS

##########################################################################

SOURCES += $$PWD/object_arx_global.cpp \
    $$PWD/ARXApplication.cpp
HEADERS += $$PWD/object_arx_global.hpp \
    $$PWD/ARXApplication.hpp

#.cpp source
SOURCES += \
        $$PWD/HellowWorld.cpp

#.h .hpp source
HEADERS += \
        $$PWD/HellowWorld.hpp


##########################
#                        #
#                        #
##########################








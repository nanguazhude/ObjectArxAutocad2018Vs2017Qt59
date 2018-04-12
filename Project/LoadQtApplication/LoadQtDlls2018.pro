
TARGET = ZFinal_LoadQtDlls

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

#use md dll
QMAKE_CXXFLAGS += /MD

#change .dll to .arx
TARGET_EXT = .arx

#remove _DEBUG
DEFINES -= _DEBUG 
DEFINES += _OBJECT_ARX_VERSION_X64_=2018

QT       += core
QT       += gui
QT       += widgets
QT       += qml

#Qt project template
TEMPLATE = lib

SOURCES += this_main.cpp

RESOURCES += \
    images.qrc

DESTDIR = $$PWD/../../release/Autocad2018

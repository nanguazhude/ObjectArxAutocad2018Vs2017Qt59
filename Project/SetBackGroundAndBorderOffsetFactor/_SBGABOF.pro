
TARGET = ZFinal_SBGABOF

#the sdk include path
INCLUDEPATH += "C:/ObjectARX 2018/inc"
INCLUDEPATH += "C:/ObjectARX 2018/inc-x64"

#rxapi.lib; acdb21.lib; acge21.lib; acad.lib; ac1st21.lib; accore.lib;
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lrxapi
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacdb23
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacge23
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lacad
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -lac1st23
LIBS+= -L"C:/ObjectARX 2018/lib-x64" -laccore

#DEF_FILE
DEF_FILE += $$PWD/AcRxDefault.def

#use md dll
QMAKE_CXXFLAGS += /MD

#change .dll to .arx
TARGET_EXT = .arx

#remove _DEBUG
DEFINES -= _DEBUG

QT       -= core
QT       -= gui

#Qt project template
TEMPLATE = lib

SOURCES += this_main.cpp



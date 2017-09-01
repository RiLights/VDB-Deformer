TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += /pipeline/pkg/houdini/16.0.671/platform-linux/arch-x86_64/ext/toolkit/include

SOURCES += \
    ../src/vdb_deformer.C

HEADERS += \
    ../src/vdb_deformer.h

DISTFILES += \
    ../package.py

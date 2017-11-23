TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += /Applications/Houdini/Houdini16.0.628/Frameworks/Houdini.framework/Versions/16.0.628/Resources/toolkit/include

SOURCES += \
    ../src/vdb_deformer.C

HEADERS += \
    ../src/vdb_deformer.h

DISTFILES += \
    ../package.py \
    ../CMakeLists.txt

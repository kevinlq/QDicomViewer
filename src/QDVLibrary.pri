include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$QDV_LIB_NAME

include(../QDicomViewer.pri)

# use precompiled header for libraries by default
isEmpty(PRECOMPILED_HEADER):PRECOMPILED_HEADER = $$PWD/shared/qDicomViewer_pch.h

DESTDIR = $$IDE_LIBRARY_PATH


osx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/Frameworks/
    QMAKE_LFLAGS += -compatibility_version $$QDICOMVIEWER_COMPAT_VERSION
}
include(rpath.pri)

TARGET = $$qtLibraryTargetName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols


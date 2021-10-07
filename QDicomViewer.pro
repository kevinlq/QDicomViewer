include($$PWD/QDicomViewer.pri)

TEMPLATE  = subdirs

#version check qt
!minQtVersion(5, 6, 2) {
    message("Cannot build QDicomViewer with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.6.2.")
}

SUBDIRS += \
    $$PWD/src \
    $$PWD/share


CONFIG   += ordered

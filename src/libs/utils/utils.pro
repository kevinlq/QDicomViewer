include(../../QDVLibrary.pri)
include(utils-lib.pri)

QT	 +=core qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:{
QT  += winextras
}

include($$PWD/Frame/BaseWidget/BaseWidget.pri)

HEADERS += \
    AppMainWidget.h \
    Frame/BaseDialog.h \
    Frame/BaseWidget.h \
    Frame/LayoutSelectPanel.h

SOURCES += \
    AppMainWidget.cpp \
    Frame/BaseDialog.cpp \
    Frame/BaseWidget.cpp \
    Frame/LayoutSelectPanel.cpp

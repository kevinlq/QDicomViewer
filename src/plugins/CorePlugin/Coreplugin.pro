DEFINES += QCORE_LIBRARY
QT += \
    network \
    printsupport \
    qml \
    sql

# embedding build time information prevents repeatedly binary exact versions from same source code
isEmpty(QDV_SHOW_BUILD_DATE): QDV_SHOW_BUILD_DATE = $$(QDV_SHOW_BUILD_DATE)
!isEmpty(QDV_SHOW_BUILD_DATE): DEFINES += QDV_SHOW_BUILD_DATE

include(../../QDVPlugin.pri)
msvc: QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

INCLUDEPATH +=$$PWD/../../

include($$PWD/actionManager/ActionManager.pri)
include($$PWD/Dialogs/Dialogs.pri)

HEADERS += \
    ContentWidgetContainer.h \
    CoreDataStruct.h \
    CoreInclude.h \
    CoreUtil.h \
    DragProgress.h \
    MainWindow.h \
    CorePlugin.h \
    ViewPanel/DImageView.h \
    coreconstants.h \
    icontext.h \
    id.h \
    icore.h \
    themechooser.h \
    coreicons.h \
    QCorePlugin_global.h \
    ListPanel/QListPanel.h \
    ViewPanel/ViewPanel.h \
    ListPanel/ListDelegate.h \
    ListPanel/DicomListView.h \
    ListPanel/QListPanelManager.h \
    ListPanel/QListPanelManagerPrivate.h \
    StatusPanel/QStatusPanel.h

SOURCES += \
    ContentWidgetContainer.cpp \
    CoreDataStruct.cpp \
    CoreUtil.cpp \
    DragProgress.cpp \
    MainWindow.cpp \
    CorePlugin.cpp \
    ViewPanel/DImageView.cpp \
    icontext.cpp \
    id.cpp \
    icore.cpp \
    themechooser.cpp \
    coreicons.cpp \
    ListPanel/QListPanel.cpp \
    ViewPanel/ViewPanel.cpp \
    ListPanel/ListDelegate.cpp \
    ListPanel/DicomListView.cpp \
    ListPanel/QListPanelManager.cpp \
    ListPanel/QListPanelManagerPrivate.cpp \
    StatusPanel/QStatusPanel.cpp


win32 {
    QT += gui-private # Uses QPlatformNativeInterface.
    LIBS += -lole32 -luser32
}
else:macx {
    LIBS += -framework AppKit
}
else:unix {
    IMAGE_SIZE_LIST = 16 24 32 48 64 128 256 512

    for(imagesize, IMAGE_SIZE_LIST) {
        eval(image$${imagesize}.files = images/logo/$${imagesize}/QtProject-qtcreator.png)
        eval(image$${imagesize}.path = $$QDV_PREFIX/share/icons/hicolor/$${imagesize}x$${imagesize}/apps)
        INSTALLS += image$${imagesize}
    }
}

RESOURCES += \
    Resources.qrc

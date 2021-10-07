include(../../QDicomViewer.pri)
include(../shared/qtsingleapplication/qtsingleapplication.pri)

TEMPLATE = app
CONFIG += qtc_runnable sliced_bundle
TARGET = $$IDE_APP_TARGET
DESTDIR = $$IDE_BIN_PATH
VERSION = $$QDICOMVIEWER_VERSION
QT -= testlib

HEADERS += ../tools/qtcreatorcrashhandler/crashhandlersetup.h
SOURCES += \
    main.cpp \
   ../tools/qtcreatorcrashhandler/crashhandlersetup.cpp

include(../rpath.pri)

LIBS *= -l$$qtLibraryName(ExtensionSystem) -l$$qtLibraryName(Aggregation) -l$$qtLibraryName(Utils)

#QT_BREAKPAD_ROOT_PATH = $$(QT_BREAKPAD_ROOT_PATH)
#!isEmpty(QT_BREAKPAD_ROOT_PATH) {
#    include($$QT_BREAKPAD_ROOT_PATH/qtbreakpad.pri)
#}
win32 {
    # We need the version in two separate formats for the .rc file
    #  RC_VERSION=4,3,82,0 (quadruple)
    #  RC_VERSION_STRING="4.4.0-beta1" (free text)
    # Also, we need to replace space with \x20 to be able to work with both rc and windres
    COPYRIGHT = "2008-$${QDICOMVIEWER_COPYRIGHT_YEAR} The Qt Company Ltd"
    DEFINES += RC_VERSION=$$replace(QDICOMVIEWER_VERSION, "\\.", ","),0 \
        RC_VERSION_STRING=\"$${QDICOMVIEWER_DISPLAY_VERSION}\" \
        RC_COPYRIGHT=\"$$replace(COPYRIGHT, " ", "\\x20")\"
    RC_FILE = App.rc
} else:macx {
    LIBS += -framework CoreFoundation
    minQtVersion(5, 7, 0) {
        QMAKE_ASSET_CATALOGS = $$PWD/qtcreator.xcassets
        QMAKE_ASSET_CATALOGS_BUILD_PATH = $$IDE_DATA_PATH
        QMAKE_ASSET_CATALOGS_INSTALL_PATH = $$INSTALL_DATA_PATH
        QMAKE_ASSET_CATALOGS_APP_ICON = qdicomviewer
    } else {
        ASSETCATALOG.files = $$PWD/qdicomviewer.xcassets
        macx-xcode {
            QMAKE_BUNDLE_DATA += ASSETCATALOG
        } else {
            ASSETCATALOG.output = $$IDE_DATA_PATH/qdicomviewer.icns
            ASSETCATALOG.commands = xcrun actool \
                --app-icon qtcreator \
                --output-partial-info-plist $$shell_quote($(TMPDIR)/qtcreator.Info.plist) \
                --platform macosx \
                --minimum-deployment-target $$QMAKE_MACOSX_DEPLOYMENT_TARGET \
                --compile $$shell_quote($$IDE_DATA_PATH) \
                $$shell_quote($$PWD/qdicomviewer.xcassets) > /dev/null
            ASSETCATALOG.input = ASSETCATALOG.files
            ASSETCATALOG.CONFIG += no_link target_predeps
            QMAKE_EXTRA_COMPILERS += ASSETCATALOG
            icns.files = \
                $$IDE_DATA_PATH/qtcreator.icns \
                $$IDE_DATA_PATH/prifile.icns \
                $$IDE_DATA_PATH/profile.icns
            icns.path = $$INSTALL_DATA_PATH
            icns.CONFIG += no_check_exist
            INSTALLS += icns
        }
    }

    infoplist = $$cat($$PWD/Info.plist, blob)
    infoplist = $$replace(infoplist, @MACOSX_DEPLOYMENT_TARGET@, $$QMAKE_MACOSX_DEPLOYMENT_TARGET)
    infoplist = $$replace(infoplist, @QDICOMVIEWER_COPYRIGHT_YEAR@, $$QDICOMVIEWER_COPYRIGHT_YEAR)
    write_file($$OUT_PWD/Info.plist, infoplist)

    QMAKE_INFO_PLIST = $$OUT_PWD/Info.plist
}

DISTFILES += App.rc \
    Info.plist \
    $$PWD/app_version.h.in

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in

CONFIG += no_batch

RESOURCES += \
    Resources.qrc

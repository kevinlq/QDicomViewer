TEMPLATE = subdirs
SUBDIRS =\
    qbs\
    qbs-create-project \
    qbs-setup-android \
    qbs-setup-toolchains \
    qbs-setup-qt \
    config \
    qbs-qmltypes

!isEmpty(QT.widgets.name):SUBDIRS += config-ui

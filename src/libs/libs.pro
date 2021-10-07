include(../../QDicomViewer.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    aggregation \
    extensionsystem \
    utils \


for(l, SUBDIRS) {
    QDV_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QDV_LIB_DEPENDS
}

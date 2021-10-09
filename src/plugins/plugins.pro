include(../../QDicomViewer.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    CorePlugin

for(p, SUBDIRS) {
    QDV_PLUGIN_DEPENDS =
    include($$p/$${p}_dependencies.pri)
    pv = $${p}.depends
    $$pv = $$QDV_PLUGIN_DEPENDS
}

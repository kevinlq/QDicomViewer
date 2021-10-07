import qbs

QbsAutotest {
    testName: "blackbox-apple"
    Depends { name: "Qt.xml" }
    Depends { name: "qbs_app" }
    Depends { name: "qbs-setup-toolchains" }
    Group {
        name: "testdata"
        prefix: "testdata-apple/"
        files: ["**/*"]
        fileTags: []
    }
    files: [
        "../shared.h",
        "tst_blackboxapple.cpp",
        "tst_blackboxapple.h",
        "tst_blackboxbase.cpp",
        "tst_blackboxbase.h",
    ]
    // TODO: Use Utilities.cStringQuote
    cpp.defines: base.concat(['SRCDIR="' + path + '"'])
}

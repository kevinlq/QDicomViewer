import qbs

Project {
    DerivedSubProject {
        filePath: "subproject.qbs"
        Properties {
            name: "something"
        }
    }
}

import qbs

Project {
    minimumQbsVersion: "1.6"
    Probe {
        id: tcProbe
        property stringList toolchain: qbs.toolchain
        property stringList targetOS: qbs.targetOS
        configure: {
            found = toolchain.contains("gcc") && targetOS.contains("unix");
            if (!found)
                console.info("project disabled");
        }
    }

    DynamicLibrary {
        condition: tcProbe.found
        name: "lib"
        property stringList defines: []
        cpp.defines: defines
        Depends { name: "cpp" }
        files: ["lib.cpp"]
    }
    CppApplication {
        condition: tcProbe.found
        name:"app"
        Depends { name: "lib" }
        files: ["main.cpp"]
    }
}

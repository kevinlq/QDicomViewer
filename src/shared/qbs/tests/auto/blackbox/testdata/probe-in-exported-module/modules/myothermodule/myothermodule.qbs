import qbs

Module {
    Depends { name: "depmodule" }
    depmodule.listProp: ["myother"]
}

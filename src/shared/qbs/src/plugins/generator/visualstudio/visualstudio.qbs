import qbs
import "../../qbsplugin.qbs" as QbsPlugin

QbsPlugin {
    name: "visualstudiogenerator"

    files: ["visualstudiogeneratorplugin.cpp"]

    Group {
        name: "Visual Studio generator"
        files: [
            "msbuildfiltersproject.cpp",
            "msbuildfiltersproject.h",
            "msbuildqbsgenerateproject.cpp",
            "msbuildqbsgenerateproject.h",
            "msbuildqbsproductproject.cpp",
            "msbuildqbsproductproject.h",
            "msbuildsharedsolutionpropertiesproject.cpp",
            "msbuildsharedsolutionpropertiesproject.h",
            "msbuildsolutionpropertiesproject.cpp",
            "msbuildsolutionpropertiesproject.h",
            "msbuildtargetproject.cpp",
            "msbuildtargetproject.h",
            "msbuildutils.h",
            "visualstudiogenerator.cpp",
            "visualstudiogenerator.h",
            "visualstudioguidpool.cpp",
            "visualstudioguidpool.h",
        ]
    }
    Group {
        name: "Solution Object Model"
        prefix: "solution/"
        files: [
            "ivisualstudiosolutionproject.cpp",
            "ivisualstudiosolutionproject.h",
            "visualstudiosolutionfileproject.cpp",
            "visualstudiosolutionfileproject.h",
            "visualstudiosolutionfolderproject.cpp",
            "visualstudiosolutionfolderproject.h",
            "visualstudiosolution.cpp",
            "visualstudiosolution.h",
            "visualstudiosolutionglobalsection.cpp",
            "visualstudiosolutionglobalsection.h",
        ]
    }
    Group {
        name: "MSBuild Object Model"
        prefix: "msbuild/"
        files: [
            "imsbuildgroup.cpp",
            "imsbuildgroup.h",
            "imsbuildnode.cpp",
            "imsbuildnode.h",
            "imsbuildnodevisitor.h",
            "imsbuildproperty.cpp",
            "imsbuildproperty.h",
            "msbuildimport.cpp",
            "msbuildimport.h",
            "msbuildimportgroup.cpp",
            "msbuildimportgroup.h",
            "msbuilditem.cpp",
            "msbuilditem.h",
            "msbuilditemdefinitiongroup.cpp",
            "msbuilditemdefinitiongroup.h",
            "msbuilditemgroup.cpp",
            "msbuilditemgroup.h",
            "msbuilditemmetadata.cpp",
            "msbuilditemmetadata.h",
            "msbuildproject.cpp",
            "msbuildproject.h",
            "msbuildproperty.cpp",
            "msbuildproperty.h",
            "msbuildpropertygroup.cpp",
            "msbuildpropertygroup.h",
        ]
    }
    Group {
        name: "MSBuild Object Model Items"
        prefix: "msbuild/items/"
        files: [
            "msbuildclcompile.cpp",
            "msbuildclcompile.h",
            "msbuildclinclude.cpp",
            "msbuildclinclude.h",
            "msbuildfileitem.cpp",
            "msbuildfileitem.h",
            "msbuildfilter.cpp",
            "msbuildfilter.h",
            "msbuildlink.cpp",
            "msbuildlink.h",
            "msbuildnone.cpp",
            "msbuildnone.h",
        ]
    }
    Group {
        name: "Visual Studio Object Model I/O"
        prefix: "io/"
        files: [
            "msbuildprojectwriter.cpp",
            "msbuildprojectwriter.h",
            "visualstudiosolutionwriter.cpp",
            "visualstudiosolutionwriter.h",
        ]
    }
}

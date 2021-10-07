/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qbs.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms and
** conditions see http://www.qt.io/terms-conditions. For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

import qbs 1.0
import qbs.File
import qbs.FileInfo
import qbs.ModUtils
import qbs.PathTools
import qbs.Probes
import qbs.Utilities
import qbs.WindowsUtils
import 'msvc.js' as MSVC

CppModule {
    condition: qbs.hostOS.contains('windows') &&
               qbs.targetOS.contains('windows') &&
               qbs.toolchain && qbs.toolchain.contains('msvc')

    id: module

    Probes.BinaryProbe {
        id: compilerPathProbe
        condition: !toolchainInstallPath
        names: ["cl"]
    }

    Probes.MsvcProbe {
        id: msvcProbe
        compilerFilePath: compilerPath
        preferredArchitecture: qbs.architecture
    }

    qbs.architecture: msvcProbe.found ? msvcProbe.architecture : original

    compilerVersionMajor: msvcProbe.versionMajor
    compilerVersionMinor: msvcProbe.versionMinor
    compilerVersionPatch: msvcProbe.versionPatch
    compilerIncludePaths: msvcProbe.includePaths

    windowsApiCharacterSet: "unicode"
    platformDefines: base.concat(WindowsUtils.characterSetDefines(windowsApiCharacterSet))
                         .concat("WIN32")
    platformCommonCompilerFlags: {
        var flags = base;
        if (compilerVersionMajor >= 18) // 2013
            flags.push("/FS");
        return flags;
    }
    compilerDefines: ['_WIN32', MSVC.compilerVersionDefine(module)]
    warningLevel: "default"
    compilerName: "cl.exe"
    compilerPath: FileInfo.joinPaths(toolchainInstallPath, compilerName)
    assemblerName: {
        switch (qbs.architecture) {
        case "armv7":
            return "armasm.exe";
        case "arm64":
            return "armasm64.exe";
        case "ia64":
            return "ias.exe";
        case "x86":
            return "ml.exe";
        case "x86_64":
            return "ml64.exe";
        }
    }

    linkerName: "link.exe"
    runtimeLibrary: "dynamic"
    separateDebugInformation: true

    property bool generateManifestFile: true
    property string toolchainInstallPath: compilerPathProbe.found ? compilerPathProbe.path
                                                                  : undefined
    architecture: qbs.architecture
    endianness: "little"
    staticLibraryPrefix: ""
    dynamicLibraryPrefix: ""
    executablePrefix: ""
    staticLibrarySuffix: ".lib"
    dynamicLibrarySuffix: ".dll"
    executableSuffix: ".exe"
    debugInfoSuffix: ".pdb"
    property string dynamicLibraryImportSuffix: ".lib"
    imageFormat: "pe"

    property var buildEnv: msvcProbe.buildEnv

    setupBuildEnvironment: {
        for (var key in buildEnv) {
            var v = new ModUtils.EnvironmentVariable(key, ';');
            v.prepend(buildEnv[key]);
            v.set();
        }
    }

    Rule {
        condition: useCPrecompiledHeader
        inputs: ["c_pch_src"]
        auxiliaryInputs: ["hpp"]
        Artifact {
            fileTags: ['obj']
            filePath: ".obj/" + Utilities.getHash(input.completeBaseName) + '_c.obj'
        }
        Artifact {
            fileTags: ['c_pch']
            filePath: ".obj/" + product.name + '_c.pch'
        }
        prepare: {
            return MSVC.prepareCompiler.apply(MSVC, arguments);
        }
    }

    Rule {
        condition: useCxxPrecompiledHeader
        inputs: ["cpp_pch_src"]
        explicitlyDependsOn: ["c_pch"]  // to prevent vc--0.pdb conflict
        auxiliaryInputs: ["hpp"]
        Artifact {
            fileTags: ['obj']
            filePath: ".obj/" + Utilities.getHash(input.completeBaseName) + '_cpp.obj'
        }
        Artifact {
            fileTags: ['cpp_pch']
            filePath: ".obj/" + product.name + '_cpp.pch'
        }
        prepare: {
            return MSVC.prepareCompiler.apply(MSVC, arguments);
        }
    }

    Rule {
        id: compiler
        inputs: ["cpp", "c"]
        auxiliaryInputs: ["hpp"]
        explicitlyDependsOn: ["c_pch", "cpp_pch"]

        Artifact {
            fileTags: ['obj']
            filePath: ".obj/" + Utilities.getHash(input.baseDir) + "/" + input.fileName + ".obj"
        }

        prepare: {
            return MSVC.prepareCompiler.apply(MSVC, arguments);
        }
    }

    FileTagger {
        patterns: ["*.manifest"]
        fileTags: ["native.pe.manifest"]
    }

    Rule {
        id: applicationLinker
        multiplex: true
        inputs: ['obj', 'native.pe.manifest']
        inputsFromDependencies: ['staticlibrary', 'dynamiclibrary_import', "debuginfo_app"]

        outputFileTags: ["application", "debuginfo_app"]
        outputArtifacts: {
            var app = {
                fileTags: ["application"],
                filePath: FileInfo.joinPaths(
                              product.destinationDirectory,
                              PathTools.applicationFilePath(product))
            };
            var artifacts = [app];
            if (product.cpp.debugInformation && product.cpp.separateDebugInformation) {
                artifacts.push({
                    fileTags: ["debuginfo_app"],
                    filePath: app.filePath.substr(0, app.filePath.length - 4)
                              + product.cpp.debugInfoSuffix
                });
            }
            return artifacts;
        }

        prepare: {
            return MSVC.prepareLinker.apply(MSVC, arguments);
        }
    }

    Rule {
        id: dynamicLibraryLinker
        multiplex: true
        inputs: ['obj', 'native.pe.manifest']
        inputsFromDependencies: ['staticlibrary', 'dynamiclibrary_import', "debuginfo_dll"]

        outputFileTags: ["dynamiclibrary", "dynamiclibrary_import", "debuginfo_dll"]
        outputArtifacts: {
            var artifacts = [
                {
                    fileTags: ["dynamiclibrary"],
                    filePath: product.destinationDirectory + "/" + PathTools.dynamicLibraryFilePath(product)
                },
                {
                    fileTags: ["dynamiclibrary_import"],
                    filePath: product.destinationDirectory + "/" + PathTools.importLibraryFilePath(product),
                    alwaysUpdated: false
                }
            ];
            if (product.cpp.debugInformation && product.cpp.separateDebugInformation) {
                var lib = artifacts[0];
                artifacts.push({
                    fileTags: ["debuginfo_dll"],
                    filePath: lib.filePath.substr(0, lib.filePath.length - 4)
                              + product.cpp.debugInfoSuffix
                });
            }
            return artifacts;
        }

        prepare: {
            return MSVC.prepareLinker.apply(MSVC, arguments);
        }
    }

    Rule {
        id: libtool
        multiplex: true
        inputs: ["obj"]
        inputsFromDependencies: ["staticlibrary", "dynamiclibrary_import"]

        Artifact {
            fileTags: ["staticlibrary"]
            filePath: product.destinationDirectory + "/" + PathTools.staticLibraryFilePath(product)
        }

        prepare: {
            var args = ['/nologo']
            var nativeOutputFileName = FileInfo.toWindowsSeparators(output.filePath)
            args.push('/OUT:' + nativeOutputFileName)
            for (var i in inputs.obj) {
                var fileName = FileInfo.toWindowsSeparators(inputs.obj[i].filePath)
                args.push(fileName)
            }
            var cmd = new Command("lib.exe", args);
            cmd.description = 'creating ' + output.fileName;
            cmd.highlight = 'linker';
            cmd.workingDirectory = FileInfo.path(output.filePath)
            cmd.responseFileUsagePrefix = '@';
            return cmd;
         }
    }

    FileTagger {
        patterns: ["*.rc"]
        fileTags: ["rc"]
    }

    Rule {
        inputs: ["rc"]
        auxiliaryInputs: ["hpp"]

        Artifact {
            filePath: ".obj/" + Utilities.getHash(input.baseDir) + "/" + input.completeBaseName + ".res"
            fileTags: ["obj"]
        }

        prepare: {
            var platformDefines = input.cpp.platformDefines;
            var defines = input.cpp.defines;
            var includePaths = input.cpp.includePaths;
            var systemIncludePaths = input.cpp.systemIncludePaths;
            var args = [];
            var i;
            var hasNoLogo = product.cpp.compilerVersionMajor >= 16; // 2010
            if (hasNoLogo)
                args.push("/nologo");

            for (i in platformDefines) {
                args.push('/d');
                args.push(platformDefines[i]);
            }
            for (i in defines) {
                args.push('/d');
                args.push(defines[i]);
            }
            for (i in includePaths) {
                args.push('/i');
                args.push(includePaths[i]);
            }
            for (i in systemIncludePaths) {
                args.push('/i');
                args.push(systemIncludePaths[i]);
            }

            args = args.concat(['/fo', output.filePath, input.filePath]);
            var cmd = new Command('rc', args);
            cmd.description = 'compiling ' + input.fileName;
            cmd.highlight = 'compiler';

            if (!hasNoLogo) {
                // Remove the first two lines of stdout. That's the logo.
                cmd.stdoutFilterFunction = function(output) {
                    var idx = 0;
                    for (var i = 0; i < 3; ++i)
                        idx = output.indexOf('\n', idx + 1);
                    return output.substr(idx + 1);
                }
            }

            return cmd;
        }
    }

    FileTagger {
        patterns: "*.asm"
        fileTags: ["asm"]
    }

    Rule {
        inputs: ["asm"]
        Artifact {
            filePath: ".obj/" + Utilities.getHash(input.baseDir) + "/" + input.completeBaseName + ".obj"
            fileTags: ["obj"]
        }
        prepare: {
            var args = ["/nologo", "/c",
                        "/Fo" + FileInfo.toWindowsSeparators(output.filePath),
                        FileInfo.toWindowsSeparators(input.filePath)];
            if (product.cpp.debugInformation)
                args.push("/Zi");
            args = args.concat(ModUtils.moduleProperty(input, 'platformFlags', 'asm'),
                               ModUtils.moduleProperty(input, 'flags', 'asm'));
            var cmd = new Command(product.cpp.assemblerPath, args);
            cmd.description = "assembling " + input.fileName;
            cmd.inputFileName = input.fileName;
            cmd.stdoutFilterFunction = function(output) {
                var lines = output.split("\r\n").filter(function (s) {
                    return !s.endsWith(inputFileName); });
                return lines.join("\r\n");
            };
            return cmd;
        }
    }
}

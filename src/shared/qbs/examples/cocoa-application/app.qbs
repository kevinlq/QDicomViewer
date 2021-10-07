/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of the examples of Qbs.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

import qbs

CppApplication {
    Depends { condition: product.condition; name: "ib" }
    condition: qbs.targetOS.contains("macos")
    name: "Cocoa Application"

    cpp.useObjcPrecompiledHeader: true
    cpp.minimumMacosVersion: "10.8"
    cpp.frameworks: ["Cocoa"]

    Group {
        prefix: "CocoaApplication/"
        files: [
            "AppDelegate.h",
            "AppDelegate.m",
            "CocoaApplication-Info.plist",
            "CocoaApplication.xcassets",
            "main.m"
        ]
    }

    Group {
        name: "Supporting Files"
        prefix: "CocoaApplication/en.lproj/"
        files: [
            "Credits.rtf",
            "InfoPlist.strings",
            "MainMenu.xib"
        ]
    }

    Group {
        files: ["CocoaApplication/CocoaApplication-Prefix.pch"]
        fileTags: ["objc_pch_src"]
    }

    Group {
        fileTagsFilter: ["bundle.content"]
        qbs.install: true
        qbs.installDir: "Applications"
        qbs.installSourceBase: product.destinationDirectory
    }

    ib.appIconName: "AppIcon"
}

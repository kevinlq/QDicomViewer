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

AppleApplicationDiskImage {
    condition: qbs.targetOS.contains("macos")
    name: "Cocoa Application DMG"
    targetName: "cocoa-application-" + version
    version: "1.0"

    Depends { name: "Cocoa Application" }
    Depends { name: "ib" }

    files: [
        "CocoaApplication/dmg.iconset",
        "CocoaApplication/en_US.lproj/LICENSE",
    ]

    // set to false to use a solid-color background (see dmg.backgroundColor below)
    property bool useImageBackground: true
    Group {
        condition: useImageBackground
        files: ["CocoaApplication/background*"]
    }

    dmg.backgroundColor: "#41cd52"
    dmg.badgeVolumeIcon: true
    dmg.iconPositions: [
        {"x": 200, "y": 200, "path": "Cocoa Application.app"},
        {"x": 400, "y": 200, "path": "Applications"}
    ]
    dmg.windowX: 420
    dmg.windowY: 250
    dmg.windowWidth: 600
    dmg.windowHeight: 422 // this *includes* the macOS title bar height of 22
    dmg.iconSize: 64
}

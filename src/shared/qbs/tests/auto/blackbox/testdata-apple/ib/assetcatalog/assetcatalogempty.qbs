import qbs
import qbs.Utilities

Project {
    property bool includeIconset

    CppApplication {
        Depends { name: "ib" }
        files: {
            var filez = ["main.c", "MainMenu.xib"];
            if (project.includeIconset)
                filez.push("empty.xcassets/empty.iconset");
            else if (Utilities.versionCompare(xcode.version, "5") >= 0)
                filez.push("empty.xcassets");
            if (qbs.hostOSVersionMinor >= 10) // need macOS 10.10 to build SBs
                filez.push("Storyboard.storyboard");
            return filez;
        }
    }
}

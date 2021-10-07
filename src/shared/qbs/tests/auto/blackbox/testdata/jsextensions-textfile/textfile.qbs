import qbs
import qbs.TextFile

Product {
    type: ["dummy"]
    Rule {
        multiplex: true
        Artifact {
            filePath: "dummy.txt"
            fileTags: ["dummy"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.silent = true;
            cmd.sourceCode = function() {
                var file1 = new TextFile("file1.txt", TextFile.WriteOnly);
                file1.write("First line.\nSecond line.\nThird line.");
                file1.close();
                file1 = new TextFile("file1.txt", TextFile.ReadWrite);
                var file2 = new TextFile("file2.txt", TextFile.WriteOnly);
                file2.writeLine(file1.atEof());
                while (true) {
                    var line = file1.readLine();
                    if (!line || line.length == 0)
                        break;
                    file2.writeLine(line);
                }
                file1.truncate();
                file2.writeLine(file1.atEof());
                file1.close();
                file2.close();
            };
            return [cmd];
        }
    }
}

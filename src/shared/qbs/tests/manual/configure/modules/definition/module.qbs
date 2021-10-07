import qbs 1.0
import qbs.Process

Module {
    name: 'definition'
    Depends { name: 'cpp' }
    Probe {
        id: node
        property string result
        configure: {
            var cmd;
            var args;
            if (qbs.targetOS.contains("windows")) {
                cmd = qbs.windowsShellPath;
                args = ["/c", "date", "/t"];
            } else {
                cmd = 'date';
                args = [];
            }
            var p = new Process();
            if (0 === p.exec(cmd, args)) {
                found = true;
                result = p.readLine();
            } else {
                found = false;
                result = undefined;
            }
            p.close();
        }
    }
    cpp.defines: node.found ? 'TEXT="Configured at ' + node.result + '"' : undefined
}

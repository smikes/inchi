var path = require('path');
var cp = require('child_process');
var os = require('os');

function run(c) {
    console.log(c);
    var p = cp.spawn(c);
    p.on('error', function(error) {
        console.log(error);
    });
    p.stdout.on('data', function(data) {
        process.stdout.write(data.toString());
    });
    p.on('close', function (code) {
        if (code) {
            process.exit(code);
        }
    });
}

function cmd_name(name) {
    return os.type() == 'Windows_NT' ? name + '.cmd' : name;
}

run(path.join('.', 'build', 'Release', 'test'));
run(path.join('.', 'node_modules', '.bin', cmd_name('mocha')));

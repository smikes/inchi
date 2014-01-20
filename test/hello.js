require('should');
var inchi = require('../lib/inchi');

describe('Hello', function() {
    describe('world', function() {
        it('should run a test', function () {
            'hello'.should.equal('hello');
        });
    });
});

describe('inchilib', function () {
    describe('version', function () {
        it('should have a version function', function () {
            (inchi.version).should.be.a.Function;
            var version = inchi.version();

            version.algorithmVersion.should.be.exactly('1');
            version.libraryVersion.should.be.exactly('1.04');
        });
        it('should have a version matching package.json', function (done) {
            var fs = require('fs');
            
            function afterRead(err, data) {
                var package = JSON.parse(data);
                var version = inchi.version();

                (version.wrapperVersion).should.be.exactly(package.version);
                done();
            }

            fs.readFile('package.json', afterRead);
        });

        it('should publish constants for return codes', function () {
            var inchilib = inchi.inchilib;

            inchilib.inchi_Ret_OKAY.should.be.exactly(0);
            inchilib.inchi_Ret_WARNING.should.be.exactly(1);
            inchilib.inchi_Ret_ERROR.should.be.exactly(2);
            inchilib.inchi_Ret_FATAL.should.be.exactly(3);
            inchilib.inchi_Ret_UNKNOWN.should.be.exactly(4);
            inchilib.inchi_Ret_BUSY.should.be.exactly(5);
            inchilib.inchi_Ret_EOF.should.be.exactly(-1);
        });
    });
});

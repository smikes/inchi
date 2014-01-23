require('should');
var inchi = require('../lib/inchi');

describe('inchi', function () {
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
    });
});

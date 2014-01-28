require('should');
var inchi = require('../lib/inchi');

describe('inchi', function () {
    describe('getVersion', function () {
        it('should have a getVersion function', function () {
            (inchi.getVersion).should.be.a.Function;
            var version = inchi.getVersion();

            version.algorithmVersion.should.be.exactly('1');
            version.libraryVersion.should.be.exactly('1.04');
        });
        it('should have a version matching package.json', function (done) {
            var fs = require('fs');

            function afterRead(err, data) {
                var package = JSON.parse(data);
                var version = inchi.getVersion();

                (version.wrapperVersion).should.be.exactly(package.version);
                done();
            }

            fs.readFile('package.json', afterRead);
        });
    });

    describe('inchilib', function () {
        it('should have a inchilib property', function () {
            (inchi.inchilib).should.be.an.Object;
        });
    });
});

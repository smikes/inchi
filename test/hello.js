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

            version.wrapperVersion.should.be.exactly('0.0.1');
            version.algorithmVersion.should.be.exactly('1');
            version.libraryVersion.should.be.exactly('1.04');
        });
    });
});

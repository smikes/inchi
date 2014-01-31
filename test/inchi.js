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

    describe('Atom', function () {
        it('should create a new atom', function () {
            var a = new inchi.Atom('H');
        });
    });

    describe('Molecule', function () {
        it('should create a new molecule', function () {
            var m = new inchi.Molecule();
        });
        it('should add some atoms', function () {
            var m = new inchi.Molecule();

            m.addAtom('C');

            (m.atoms.length).should.be.exactly(1);
            (m.atoms[0]).should.be.exactly('C');
        });
        it('should make bonds', function () {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('O');
            m.addBond(0, 1);

            (m.bonds.length).should.be.exactly(1);
        });
        it('should retrieve InChI code for methane', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/CH4/h1H4');
                done();
            });
        });
    });
});

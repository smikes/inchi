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

            a.getName().should.equal('H');
        });
        it('should rename a new atom', function () {
            var a = new inchi.Atom('H');

            a.getName().should.equal('H');
            a.setName('C');
            a.getName().should.equal('C');
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
        it('should record default order of 1 for bonds', function () {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('O');
            m.addBond(0, 1);

            (m.bonds[0]).order.should.be.exactly(1);
        });
        it('should record order for bonds', function () {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('O');
            m.addBond(0, 1, 2);

            (m.bonds[0]).order.should.be.exactly(2);
        });
        it('should retrieve InChI code for methane', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/CH4/h1H4');
                done();
            });
        });
        it('should retrieve InChI code for methanol', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('O');
            m.addBond(0, 1);

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/CH4O/c1-2/h2H,1H3');
                done();
            });
        });

        it('should retrieve InChI code for formaldehyde', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('O');
            m.addBond(0, 1, 2);

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/CH2O/c1-2/h1H2');
                done();
            });
        });

        it('should retrieve InChI code for acetylene', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('C');
            m.addBond(0, 1, 3);

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/C2H2/c1-2/h1-2H');
                done();
            });
        });

        it('should retrieve InChI code for benzene', function (done) {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('C');
            m.addAtom('C');
            m.addAtom('C');
            m.addAtom('C');
            m.addAtom('C');
            m.addBond(0, 1);
            m.addBond(1, 2, 2);
            m.addBond(2, 3);
            m.addBond(3, 4, 2);
            m.addBond(4, 5);
            m.addBond(5, 0, 2);

            m.getInchi(function(err, i) {
                i.should.startWith('InChI=1S/C6H6/c1-2-4-6-5-3-1/h1-6H');
                done();
            });
        });

        it('should convert InChI code to molecule', function (done) {
            inchi.Molecule.fromInchi('InChI=1S/CH4O/c1-2/h2H,1H3', function (err, mol) {
                (mol.atomCount()).should.be.exactly(2);
                (mol.atoms[0]).should.be.exactly("C");
                (mol.atoms[1]).should.be.exactly("O");

                done();
            });
        });

        function roundTrip(i1, done) {
            inchi.Molecule.fromInchi(i1, function (e1, mol) {
                mol.getInchi(function (e2, i2) {

                    i2.should.be.exactly(i1);
                    done();
                });
            });
        }

        it('should be able to make round-trips (methanol)', function (done) {
            var methanol = 'InChI=1S/CH4O/c1-2/h2H,1H3';

            roundTrip(methanol, done);
        });

        it('should be able to make round-trips (benzene)', function (done) {
            var benzene = 'InChI=1S/C6H6/c1-2-4-6-5-3-1/h1-6H';

            roundTrip(benzene, done);
        });

        it('should be able to make round-trips (caffeine)', function (done) {
            var caffeine = 'InChI=1S/C8H10N4O2/c1-10-4-9-6-5(10)7(13)12(3)8(14)11(6)2/h4H,1-3H3';

            roundTrip(caffeine, done);
        });

        it('should be able to make round-trips (keto-D-fructose)', function (done) {
            var fructose = 'InChI=1S/C6H12O6/c7-1-3(9)5(11)6(12)4(10)2-8/h3,5-9,11-12H,1-2H2/t3-,5-,6-/m1/s1';

            roundTrip(fructose, done);
        });

    });
});

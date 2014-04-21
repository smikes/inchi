require('should');
var inchi = require('../lib/inchi'),
    molfile = require('molfile');

function roundTrip(i1, done) {
    inchi.Molecule.fromInchi(i1, function (e1, mol) {
        mol.getInchi(function (e2, i2) {

            if (i1 !== i2) {
                console.log(mol);
            }

            i2.should.be.exactly(i1);
            done();
        });
    });
}

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

            m.addAtom({elname: 'C'});

            (m.atoms.length).should.be.exactly(1);
            (m.atoms[0]).should.have.property('elname', 'C');
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

            m.addAtom({elname: 'C'});
            m.addAtom({elname: 'O'});
            m.addBond(0, 1);

            (m.bonds[0]).order.should.be.exactly(1);
        });
        it('should record order for bonds', function () {
            var m = new inchi.Molecule();

            m.addAtom({elname: 'C'});
            m.addAtom({elname: 'O'});
            m.addBond(0, 1, 2);

            (m.bonds[0]).order.should.be.exactly(2);
        });
        it('should retrieve InChI code for methane', function (done) {
            var m = new inchi.Molecule();

            m.addAtom({elname: 'C'});

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

        it('should retrieve InChI code for argon', function (done) {
            var m = new inchi.Molecule;

            m.addAtom({ x: 10.2967, y: -1.5283, z: -15283, elname: 'Ar' });
            m.getInchi(function (err, i) {
                i.should.equal('InChI=1S/Ar');
                done();
            });
        });

        it('should convert InChI code to molecule', function (done) {
            inchi.Molecule.fromInchi('InChI=1S/CH4O/c1-2/h2H,1H3', function (err, mol) {
                (mol.atomCount()).should.be.exactly(2);
                (mol.atoms[0]).should.have.property('elname', "C");
                (mol.atoms[1]).should.have.property('elname', "O");

                done();
            });
        });

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

        it('should be able to make round-trips (cholesterol)', function (done) {
            var cholesterol = 'InChI=1S/C27H46O/c1-18(2)7-6-8-19(3)23-11-12-24' +
                    '-22-10-9-20-17-21(28)13-15-26(20,4)25(22)14-16-27(23,24)5' +
                    '/h9,18-19,21-25,28H,6-8,10-17H2,1-5H3/t19-,21+,22+,23-,24+' +
                    ',25+,26+,27-/m1/s1';

            roundTrip(cholesterol, done);
        });

        it('should be able to make round-trips (elements)', function (done) {
            var elements = 'InChI=1S/CH4.Ac.Ag.Al.Am.Ar.AsH3.AtH.Au.BH3.Ba.Be.Bi' +
                    '.Bk.BrH.Ca.Cd.Ce.Cf.ClH.Cm.Co.Cr.Cs.Cu.Dy.Er.Es.Eu.FH.Fe.Fm' +
                    '.Fr.Ga.Gd.GeH4.He.Hf.Hg.Ho.HI.In.Ir.K.Kr.La.Li.Lr.Lu.Md.Mg.' +
                    'Mn.Mo.H3N.Na.Nb.Nd.Ne.Ni.No.Np.H2O.Os.H3P.Pa.Pb.Pd.Pm.Po.Pr' +
                    '.Pt.Pu.Ra.Rb.Re.Rf.Rh.Rn.Ru.H2S.Sb.Sc.H2Se.H4Si.Sm.Sn.Sr.Ta' +
                    '.Tb.Tc.H2Te.Th.Ti.Tl.Tm.U.V.W.Xe.Y.Yb.Zn.Zr.3H2.40H/h1H4;;;' +
                    ';;;1H3;1H;;1H3;;;;;1H;;;;;1H;;;;;;;;;;1H;;;;;;1H4;;;;;1H;;;' +
                    ';;;;;;;;;;1H3;;;;;;;;1H2;;1H3;;;;;;;;;;;;;;;;1H2;;;1H2;1H4;' +
                    ';;;;;;1H2;;;;;;;;;;;;;3*1H;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;' +
                    ';;;;;;;/i;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;' +
                    ';;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1+2;1+' +
                    '1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;';

            roundTrip(elements, done);
        });

        it('should be able to make round-trips (elements2)', function (done) {
            var elements = 'InChI=1S/3CH3.Ac.Ag.Al.Am.ArCl.AsH3Se.AtRn.Au.BH3.Ba.Be.Bi.Bk.BrKr.Ca.Cd.Ce.Cf.Cm.Co.Cr.Cs.Cu.Dy.Er.Es.Eu.FNe.Fe.Fm.Fr.Ga.Gd.GeH3.He.Hf.Hg.Ho.IXe.In.Ir.K.La.Li.Lr.Lu.Md.Mg.Mn.Mo.H2N.Na.Nb.Nd.Ni.No.Np.H2O.Os.H3PS.Pa.Pb.Pd.Pm.Po.Pr.Pt.Pu.Ra.Rb.Re.Rf.Rh.Ru.Sb.Sc.H3Si.Sm.Sn.Sr.Ta.Tb.Tc.H2Te.Th.Ti.Tl.Tm.U.V.W.Y.Yb.Zn.Zr.21H/c;;;;;;;3*1-2;;;;;;;1-2;;;;;;;;;;;;;;1-2;;;;;;;;;;;1-2;;;;;;;;;;;;;;;;;;;;;1-2;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;/h3*1H3;;;;;;2H,1H2;;;1H3;;;;;;;;;;;;;;;;;;;;;;;;;1H3;;;;;;;;;;;;;;;;;1H2;;;;;;;1H2;;2H,1H2;;;;;;;;;;;;;;;;;1H3;;;;;;;1H2;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;/q;;;+1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;-1;;;;;;;;;;;;;;;;;;+1;;;;;;+1;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;/p-2/i;;;;;;;;;;;1T;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1+1;;;;;;;;;;;;;;;;;;;;';

            roundTrip(elements, done);
        });

        it('should handle isotopes specified directly', function (done) {
            var m = new inchi.Molecule;

            m.addAtom('C');
            m.addAtom({elname: 'H', isotopic_mass: 2});
            m.addBond(0, 1);

            m.getInchi(function(err, i) {
                i.should.equal("InChI=1S/CH4/h1H4/i1D");
                done();
            });
        });

        it('should handle isotopes specified by shift', function (done) {
            var m = new inchi.Molecule;

            m.addAtom('C');
            m.addAtom({elname: 'H', isotopic_mass: 1 + inchi.ISOTOPIC_SHIFT_FLAG});
            m.addBond(0, 1);

            m.getInchi(function(err, i) {
                i.should.equal("InChI=1S/CH4/h1H4/i1D");
                done();
            });
        });

        it('should handle isotopes specified by iso_H', function (done) {
            var m = new inchi.Molecule;

            m.addAtom('C');
            m.atoms[0].num_iso_H = [3, 0, 1, 0];

            m.getInchi(function(err, i) {
                i.should.equal("InChI=1S/CH4/h1H4/i1D");
                done();
            });
        });
    });

    describe('ill-formed molecule', function () {
        it('should complain about bonds between nonexistent atoms', function () {
            var m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('C');

            (function () { m.addBond(1, 2); }).should.throw(/out of range/);
            (function () { m.addBond(2, 1); }).should.throw(/out of range/);
        });
    });

    describe('moleculeFromMolfile', function () {
        it('should handle simple mol file (methane)', function () {
            var z = {
                atoms: [{elname: 'C'}],
                bonds: [],
                properties: {}
            },
                m = inchi.moleculeFromMolfile(z);

            (m.atoms.length).should.equal(1);
            (m.atoms[0].num_iso_H === undefined).should.be.true;
        });

        it('should handle valenceCode (methyl)', function () {
            var z = {
                atoms: [{elname: 'C', valenceCode: 3}],
                bonds: [],
                properties: {}
            },
                m = inchi.moleculeFromMolfile(z);

            (m.atoms.length).should.equal(1);
            (m.atoms[0].num_iso_H[0]).should.equal(3);
        });

        it('should handle valenceCode (ethyl)', function () {
            var z = {
                atoms: [{elname: 'C', valenceCode: 3},
                        {elname: 'C'}],
                bonds: [{from:1, to:2, bondType:1}],
                properties: {}
            },
                m = inchi.moleculeFromMolfile(z);

            (m.atoms[0].num_iso_H[0]).should.equal(2);
        });

        it('should cound bonds (ethyl)', function () {
            var z = {
                atoms: [{elname: 'C', valenceCode: 3},
                        {elname: 'C'}],
                bonds: [{from:1, to:2, bondType:1}],
                properties: {}
            };

            (inchi.countMolfileBonds(z, 0)).should.equal(1);
            (inchi.countMolfileBonds(z, 1)).should.equal(1);
        });
    });

    describe('real-life data', function () {
        it('should read Elements_C2', function (done) {
            var fs = require('fs'),
                molfile = require('molfile'),
                file = fs.readFileSync("test/fixtures/Elements_C2.sdf"),
                mol = molfile.parseMol(String(file)),
                m = inchi.moleculeFromMolfile(mol),
                inchiMol = new inchi.inchilib.Molecule(m);

            inchiMol.GetInChI(function (err, inchi) {
                done();
            });
        });
        it('should handle a copper-lawrencium bond', function (done) {
            var m = new inchi.Molecule;
            m.addAtom('Cu');
            m.addAtom('Lr');
            m.addBond(0, 1);

            m.getInchi(function (err, inchi) {
                inchi.should.equal('InChI=1S/Cu.Lr');
                done();
            });
        });
        it('should handle zwitterions_1.#002', function (done) {
            var m = new inchi.Molecule();
            m.addAtom('C');
            m.addAtom('C');
            m.addAtom({elname: 'N', charge: 1});
            m.addAtom({elname: 'N', charge: -1});
            m.addBond(0, 1);
            m.addBond(1, 2, 3); // C#N triple
            m.addBond(2, 3);

            m.getInchi(function (err, i) {
                i.should.equal('InChI=1S/C2H4N2/c1-2-4-3/h3H,1H3');
                done();
            });
        });
        it('should generate the right molecule from zwitterions_1.#002', function (done) {
            var zwitter = '{"header":{"name":"","initials":"  ","software":"Mol2Comp","date":"2006-06-19T00:07:00.000Z","comment":""},"countLine":{"atoms":4,"bonds":3,"chiral":0,"mLines":999,"version":" V2000"},"atoms":[{"x":6.7542,"y":1.9708,"z":0,"elname":"C","massDiff":0,"chargeCode":0,"valenceCode":0},{"x":7.5792,"y":1.9708,"z":0,"elname":"C","massDiff":0,"chargeCode":0,"valenceCode":0},{"x":8.4042,"y":1.9708,"z":0,"elname":"N","massDiff":0,"chargeCode":3,"valenceCode":0},{"x":9.2292,"y":1.9708,"z":0,"elname":"N","massDiff":0,"chargeCode":5,"valenceCode":0}],"bonds":[{"from":1,"to":2,"bondType":1},{"from":2,"to":3,"bondType":3},{"from":3,"to":4,"bondType":1}],"properties":{"CHG":{"3":1,"4":-1},"":{}},"data":{"ID":"zwitterions_1.#002"}}',
                z = JSON.parse(zwitter);
            var m = inchi.moleculeFromMolfile(z);

            (m.atoms.length).should.equal(4);

            m.getInchi(function (err, i) {
                i.should.equal('InChI=1S/C2H4N2/c1-2-4-3/h3H,1H3');
                done();
            });
        });
        it('can round trip an ylide', function (done) {
            var ylide = 'InChI=1S/C3H9NOS/c1-3-4(2,5)6/h6H,3H2,1-2H3/t4-/m0/s1';

            roundTrip(ylide, done);

        });
        it('should generate the right inchi from ylide_to_dbond_effect.#001', function (done) {
            var expected = 'InChI=1S/C3H9NOS/c1-3-4(2,5)6/h6H,3H2,1-2H3/t4-/m0/s1',
                m = new inchi.Molecule();

            m.addAtom('C');
            m.addAtom('C');
            m.addAtom('C');
            m.addAtom({elname: 'N', charge: 1});
            m.addAtom({elname: 'O', charge: -1});
            m.addAtom('S');

            m.addBond(0, 3);
            m.addBond(1, 3);
            m.addBond(2, 3);
            m.addBond(3, 4);
            m.bonds[3].stereo = 1;

            m.addBond(3, 5);

            console.log(JSON.stringify(m,null,2));

            m.getInchi(function (err, i) {
                i.should.equal(expected);
            });
        });

        it('should support isotopes', function (done) {
            // Tech_Man_Figure19
            var sdf = '\n  -ClnMol-06180618052D\n\n  6  5  0  0  0  0  0  0  0  0999 V2000\n    6.1181   -5.5901    0.0000 O   0  0  0  0  0  0  0  0  0  0  0  0\n    6.1181   -6.6327    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0\n    7.0292   -7.1586    0.0000 N   0  0  3  0  0  0  0  0  0  0  0  0\n    5.2211   -7.1540    0.0000 N   0  0  0  0  0  0  0  0  0  0  0  0\n    7.9375   -6.6292    0.0000 H   1  0  0  0  0  0  0  0  0  0  0  0\n    7.3000   -8.1708    0.0000 H   1  0  0  0  0  0  0  0  0  0  0  0\n  2  3  1  0  0  0  0\n  2  4  1  0  0  0  0\n  3  5  1  0  0  0  0\n  1  2  2  0  0  0  0\n  3  6  1  0  0  0  0\nM  ISO  2   5   2   6   2\nM  END\n>  <ID>\n_Tech_Man_Figure19 \n\n$$$$\n',
                expected = 'InChI=1S/CH4N2O/c2-1(3)4/h(H4,2,3,4)/i/hD2',
                parsedMol = molfile.parseMol(sdf),
                m = inchi.moleculeFromMolfile(parsedMol);

            (parsedMol.atoms.length).should.equal(6);
            console.log(parsedMol.atoms[5]);

            console.log(m.atoms[5]);

            m.getInchi(function (err, i) {
                i.should.equal(expected);
                done();
            });
        });

        it('should support stereochem', function (done) {
            var sdf = '\n  Mol2Comp06180618082D\n\n  5  4  0  0  0  0  0  0  0  0999 V2000\n    1.7773   -5.6172    0.0000 F   0  0  0  0  0  0  0  0  0  0  0  0\n    2.7635   -6.2512    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0\n    3.7967   -5.6500    0.0000 I   0  0  0  0  0  0  0  0  0  0  0  0\n    2.7588   -7.4487    0.0000 Cl  0  0  0  0  0  0  0  0  0  0  0  0\n    2.7588   -5.0536    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0\n  1  2  1  0  0  0  0\n  2  3  1  0  0  0  0\n  2  4  1  0  0  0  0\n  2  5  1  1  0  0  0\nM  END\n>  <ID>\n_Tech_Man_Table09.#001 \n\n$$$$\n',
                expected = 'InChI=1S/C2H3ClFI/c1-2(3,4)5/h1H3/t2-/m1/s1',
                parsedMol = molfile.parseMol(sdf),
                m = inchi.moleculeFromMolfile(parsedMol);

            (parsedMol.atoms.length).should.equal(5);
            console.log(parsedMol);

            console.log(m);

            m.getInchi(function (err, i) {
                i.should.equal(expected);
                done();
            });

        });

    });
});

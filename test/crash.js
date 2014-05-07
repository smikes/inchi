require('should');
var inchi = require('..'),
    inchilib = inchi.inchilib,
    molfile = require('molfile');

describe('invalid inputs to GetStructFromINCHI', function () {
    it('should error when an invalid inchi is supplied', function (done) {
        inchilib.GetStructFromINCHI('💩', function (err, struct) {
            (struct.result).should.equal(inchilib.inchi_Ret_EOF);
            done();
        });
    });

    it('should error when a truncated inchi is supplied', function (done) {
        inchilib.GetStructFromINCHI('InChI=1S/', function (err, struct) {
            (struct.result).should.equal(inchilib.inchi_Ret_EOF);
            done();
        });
    });

    it('should error when an invalid atom name is supplied', function (done) {
        inchilib.GetStructFromINCHI('InChI=1S/CO/c2-3,2-3', function (err, struct) {
            (struct.result).should.equal(inchilib.inchi_Ret_EOF);
            done();
        });
    });

    describe('should error when an invalid structure is supplied', function () {

        it('should expose checkValidStruct', function () {
            (function () {
                inchi.checkValidStruct({});
            }).should.throw(/Missing array/);
        });

        it('should detect non-objects', function (done) {
            inchilib.GetINCHI('foo', function (err, struct) {
                (err.message).should.match(/Expected object/);
                done();
            });
        });

        it('should fail gracefully when subobjects are missing (atoms)', function (done) {
            inchilib.GetINCHI({bonds: [], stereo0D: []}, function (err, struct) {
                (err.message).should.match(/Missing array/);
                done();
            });
        });

        it('should fail gracefully when atoms are incorrectly specified', function (done) {
            inchilib.GetINCHI({atoms: ['a'], bonds:[], stereo0D: []}, function (err, struct) {
                (err.message).should.match(/Expected object: atoms\[0\]/);
                done();
            });
        });

        it('should fail gracefully when atom name is missing', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {}],
                bonds:[], stereo0D: []
            }, function (err, struct) {
                (err.message).should.match(/Missing elname: atoms\[1\]/);
                done();
            });
        });

        it('should fail gracefully when bond is invalid', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}],
                bonds:[3], stereo0D: []
            }, function (err, struct) {
                (err.message).should.match(/Expected object: bonds\[0\]/);
                done();
            });
        });

        it('should fail gracefully when bond is invalid', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}],
                bonds:[ {from: 3, to: 'ernie', type: -2} ],
                stereo0D: []
            }, function (err, struct) {
                (err.message).should.match(/Bond 'from' out of range/);
                done();
            });
        });

        it('should fail gracefully when bond has weird types', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {elname: 'O'}],
                bonds:[ {from: 0, to: 1, type: -2, stereo: 1} ],
                stereo0D: []
            }, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_FATAL);
                done();
            });
        });

        it('should fail gracefully when bond "from" refers to out-of-range atom', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {elname: 'O'}],
                bonds:[ {from: -99, to: 10, order: -2, stereo: 1} ],
                stereo0D: []
            }, function (err, struct) {
                (err.message).should.match(/Bond 'from' out of range/);
                done();
            });
        });

        it('should fail gracefully when bond "to" refers to out-of-range atom', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {elname: 'O'}],
                bonds:[ {from: 0, to: 10, order: -2, stereo: 1} ],
                stereo0D: []
            }, function (err, struct) {
                (err.message).should.match(/Bond 'to' out of range/);
                done();
            });
        });

        it('should fail gracefully when bondtype is weird', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {elname: 'O'}],
                bonds:[ {from: 0, to: 1, order: -2, stereo: -9} ],
                stereo0D: []
            }, function (err, res) {
                (res.result).should.equal(inchi.inchilib.inchi_Ret_FATAL);
                (res.message).should.match(/Unrecognized bond type:/);
                done();
            });
        });

        it('should fail gracefully when bond stereo is weird', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}, {elname: 'O'}],
                bonds:[ {from: 0, to: 1, order: 3, stereo: -9} ],
                stereo0D: []
            }, function (err, res) {
                (res.result).should.equal(inchi.inchilib.inchi_Ret_FATAL);
                (res.message).should.match(/Unrecognized bond stereo:/);
                done();
            });
        });

        it('should fail gracefully when stereo is invalid', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}],
                bonds:[],
                stereo0D: ['foo']
            }, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_WARNING);
                (struct.inchi).should.equal('InChI=1S/CH4/h1H4');
                (struct.message).should.match(/Wrong 0D stereo descriptor/);
                done();
            });
        });

        it('should fail gracefully when stereo is invalid (2)', function (done) {
            inchilib.GetINCHI({
                atoms: [{elname: 'C'}],
                stereo0D: ['foo']
            }, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_WARNING);
                (struct.inchi).should.equal('InChI=1S/CH4/h1H4');
                (struct.message).should.match(/Wrong 0D stereo descriptor/);
                done();
            });
        });

        it('should fail gracefully when stereo refers to nonexistent atom', function (done) {
            var mol = {
                atoms: [{elname: 'C'}],
                bonds:[],
                stereo0D: [{
                    neighbor: [-5, 17, 137.3, 9999999999999999999],
                    central_atom: -99,
                    type: 2,
                    parity: 4
                }]
            };

            inchilib.GetINCHI(mol, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_WARNING);
                (struct.inchi).should.equal('InChI=1S/CH4/h1H4');
                (struct.message).should.match(/Wrong 0D stereo descriptor/);
                done();
            });
        });

        it('should fail gracefully when too many atoms', function (done) {
            var mol = {
                atoms: [{elname: 'C'}],
                bonds:[],
                stereo0D: []
            };

            for(var i = 0; i < 1025; i += 1) {
                mol.atoms.push(mol.atoms[0]);
            }

            inchilib.GetINCHI(mol, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_ERROR);
                (struct.message).should.match(/Too many atoms/);
                done();
            });
        });

        it('should fail gracefully when bond to nonexistent atom', function (done) {
            var mol = {
                atoms: [{elname: 'C'}, {elname: 'C'}],
                bonds:[],
                stereo0D: []
            };

            mol.bonds.push({from: 0, to: 2, order: 1});

            inchilib.GetINCHI(mol, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_FATAL);
                (struct.message).should.match(/Bond to nonexistent atom/);
                done();
            });
        });

        it('should fail gracefully when too many bonds', function (done) {
            var mol = {
                atoms: [{elname: 'C'}],
                bonds:[],
                stereo0D: []
            };

            for (var i = 0; i < 1022; i += 1) {
                mol.atoms.push(mol.atoms[0]);
            }

            for (var i = 2; i < 1019; i += 1) {
                mol.bonds.push({from: i, to: i-2, order: 1});
                mol.bonds.push({from: i, to: i-1, order: 1});
                mol.bonds.push({from: i, to: i+1, order: 1});
                mol.bonds.push({from: i, to: i+2, order: 1});
            }

            inchilib.GetINCHI(mol, function (err, struct) {
                (struct.result).should.equal(inchi.inchilib.inchi_Ret_WARNING);
                (struct.message).should.match(/Omitted undefined stereo/);
                done();
            });
        });

    });
});

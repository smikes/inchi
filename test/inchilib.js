require('should');
var inchi = require('../lib/inchi'),
    inchilib = inchi.inchilib;

describe('inchilib', function () {
    describe('constants', function () {
        it('should publish constants for return codes', function () {
            inchilib.inchi_Ret_OKAY.should.be.exactly(0);
            inchilib.inchi_Ret_WARNING.should.be.exactly(1);
            inchilib.inchi_Ret_ERROR.should.be.exactly(2);
            inchilib.inchi_Ret_FATAL.should.be.exactly(3);
            inchilib.inchi_Ret_UNKNOWN.should.be.exactly(4);
            inchilib.inchi_Ret_BUSY.should.be.exactly(5);
            inchilib.inchi_Ret_EOF.should.be.exactly(-1);
        });
    });

    describe('GetINCHI', function () {

        it('should convert simple molecules correctly, asynchronously', function (done) {
            var methanol = {
                atom: [
                    { elname: 'C', neighbor: [1] },
                    { elname: 'O', neighbor: [0] }
                ]
            };

            function callback(err, result) {
                (result.inchi).should.startWith('InChI=1S/CH4O/c1-2/h2H,1H3');
                (result.auxinfo).should.startWith('AuxInfo=1/0/N:1,2/rA:2CO');
                (result.code).should.be.exactly(inchilib.inchi_Ret_OKAY);
                (result.message).should.equal(''); // no error message
                (result.log).should.startWith('Generating standard InChI\nInput format: MOLfile\n' +
                                              'Output format: Plain text\nFull Aux. info\n' +
                                              'No timeoutUp to 1024 atoms per structure');

                done();
            }

            inchilib.GetINCHI(methanol, callback);
        });
    });

    describe('GetStructFromINCHI', function () {
        it('should be a function', function () {
            (inchilib.GetStructFromINCHI).should.be.a.Function;
        });

        it('should convert InChI to JSON representation of inchi_OutputStruct', function(done) {
            inchilib.GetStructFromINCHI('InChI=1S/CH4O/c1-2/h2H,1H3', function (err, obj) {

                obj.should.be.an.Object;
                obj.atom[0].should.have.property('elname', 'C');
                obj.atom[1].should.have.property('elname', 'O');
                obj.atom[0].bonds.should.have.length(1);
                obj.atom[0].bonds[0].neighbor.should.be.exactly(1);
                obj.atom[0].bonds[0].bond_type.should.be.exactly(1);

                done();
            });
        });

    });

    describe('Molecule', function () {

        it('should publish a Molecule constructor', function () {
            (inchilib.Molecule).should.be.a.Function;
        });
        it('should create an empty Molecule', function () {
            var m = new inchilib.Molecule({atoms: [], bonds: []});
        });
        it('should have a GetInChI method', function () {
            var m = new inchilib.Molecule({atoms: [], bonds: []});

            (m.GetInChI).should.be.a.Function;
        });
        it('should not assert when constructor is called incorrectly', function () {
            (function () {
                inchilib.Molecule({atoms: [], bonds: []})
            }).should.throw(/new operator/);
        });
        it('should not assert when Molecule::GetInChI is called on non-molecule', function () {
            (function () {
                var foo = { GetInChI: inchilib.Molecule.prototype.GetInChI };
                foo.GetInChI(function () {
                });
            }).should.throw(/member function of Molecule/);
        });
        it('should not assert when Molecule::GetInChI is called without context', function () {
            (function () {
                var func = inchilib.Molecule.prototype.GetInChI;
                func();
            }).should.throw(/member function of Molecule/);
        });
    });

});

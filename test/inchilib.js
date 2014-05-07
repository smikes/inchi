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
                obj.atom[0].bonds[0].neighbor.should.equal(1);
                obj.atom[0].bonds[0].bond_type.should.equal(1);

                done();
            });
        });

    });

});

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

});

require('should');
var inchi = require('../lib/inchi'),
    inchilib = inchi.inchilib;

describe('inchilib', function () {
    it('should publish constants for return codes', function () {
        inchilib.inchi_Ret_OKAY.should.be.exactly(0);
        inchilib.inchi_Ret_WARNING.should.be.exactly(1);
        inchilib.inchi_Ret_ERROR.should.be.exactly(2);
        inchilib.inchi_Ret_FATAL.should.be.exactly(3);
        inchilib.inchi_Ret_UNKNOWN.should.be.exactly(4);
        inchilib.inchi_Ret_BUSY.should.be.exactly(5);
        inchilib.inchi_Ret_EOF.should.be.exactly(-1);
    });
    it('should publish the "classic" API', function () {
        (inchilib.GetINCHISync).should.be.a.Function;

        // No need to publish FreeINCHI, as the return from GetINCHI is
        // converted to a v8 object and FreeINCHI is called inside C++
    });
    it('should convert simple molecules correctly', function () {
        var methanol = {
            atom: [
                { elname: 'C', neighbor: [1] },
                { elname: 'O', neighbor: [0] }
            ]
        };

        var result = inchilib.GetINCHISync(methanol);

        (result.inchi).should.startWith('InChI=1S/CH4O/c1-2/h2H,1H3');
        (result.auxinfo).should.startWith('AuxInfo=1/0/N:1,2/rA:2CO');
        (result.code).should.be.exactly(inchilib.inchi_Ret_OKAY);
        (result.message).should.equal(''); // no error message
        (result.log).should.startWith('Generating standard InChI\nInput format: MOLfile\n' +
                                      'Output format: Plain text\nFull Aux. info\n' +
                                      'No timeoutUp to 1024 atoms per structure');
    });
    it('should not leak memory when converting molecules', function() {

        var methanol = {
            atom: [
                { elname: 'C', neighbor: [1] },
                { elname: 'O', neighbor: [0] }
            ]
        },
            i;

        for (i = 0; i < 10; i += 1) {
            inchilib.GetINCHISync(methanol);
        }
    });
    it('should make reasonable complaints for incorrect molecules', function () {
        var invalid = {
            atom: [
                { elname: 'Foo', neighbor: [1] },
                { elname: 'Bar', neighbor: [0] }
            ]
        };

        var result = inchilib.GetINCHISync(invalid);

        (result.inchi).should.be.exactly('');
        (result.auxinfo).should.be.exactly('');
        (result.code).should.be.exactly(inchilib.inchi_Ret_ERROR);
        (result.message).should.equal('Unknown element(s): Foo');
    });
    it('should not crash on empty molecule', function () {
        var empty = {
            atom: [
            ]
        };

        var result = inchilib.GetINCHISync(empty);

        (result.inchi).should.be.exactly('');
        (result.auxinfo).should.be.exactly('');
        (result.code).should.be.exactly(inchilib.inchi_Ret_EOF);
        (result.message).should.equal('');
    });
    it('should not crash on malformed input', function () {
        var noAtoms = {
        };

        var result = inchilib.GetINCHISync(noAtoms);

        (result.inchi).should.be.exactly('');
        (result.auxinfo).should.be.exactly('');
        (result.code).should.be.exactly(inchilib.inchi_Ret_EOF);
        (result.message).should.equal('');
    });

});

require('should');
var inchi = require('../lib/inchi'),
    m2i = inchi.molfile,
    molfile = require('molfile');


describe('molfileToInchi', function () {
    it('should count bonds (ethyl)', function () {
        var z = {
            atoms: [{elname: 'C', valenceCode: 3},
                    {elname: 'C'}],
            bonds: [{from:1, to:2, bondType:1}],
            properties: {}
        };

        (m2i.countMolfileBonds(z, 0)).should.equal(1);
        (m2i.countMolfileBonds(z, 1)).should.equal(1);
    });

    it('should calculate isotopic hydrogen count', function () {
        // valence code 15: zero valence
        (m2i.makeNumIsoH(15,0)).should.eql([0, 0, 0, 0]);

        // valence code 0: default valence
        (m2i.makeNumIsoH(0,0)).should.eql([-1, 0, 0, 0]);

        // valence code 1-14: explicit valence, less bonds
        (m2i.makeNumIsoH(3,0)).should.eql([3, 0, 0, 0]);
        (m2i.makeNumIsoH(3,1)).should.eql([2, 0, 0, 0]);
        (m2i.makeNumIsoH(3,3)).should.eql([0, 0, 0, 0]);
        (m2i.makeNumIsoH(3,4)).should.eql([0, 0, 0, 0]);
    });

});

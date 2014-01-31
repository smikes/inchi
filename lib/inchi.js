/* javascript wrapper for inchi */

(function () {
    'use strict';

    var inchilib = require('bindings')('inchi.node'),
        Molecule;

    /**
     * High-level javascript wrapper of InChI API
     *
     * @module InChI
     * @class  InChI
     */

    /**
     * Reports version of this software.  Because there are three
     * independent pieces, an object is returned, for example:

          {
             wrapperVersion: '1.0.1', // version of javascript wrapper
             libraryVersion: '1.04',  // version of C/C++ implementation
             algorithmVersion: '1'    // version of InChI algorithm
          }


     * @method getVersion
     * @return {Object} Object containing versions
     */
    exports.getVersion = function () {
        return {
            wrapperVersion: '0.0.5pre',
            algorithmVersion: inchilib.getAlgorithmVersion(),
            libraryVersion: '1.04'
        };
    };

    /**
     * Provides direct access to the low-level C++ API
     *
     * @property inchilib
     * @type InChILib
     */
    exports.inchilib = inchilib;

    /**
     @class Molecule
     @module InChI
     */
    Molecule = function () {
        this.atoms = [];
        this.bonds = [];
    };
    exports.Molecule = Molecule;

    /**
     Add an atom to the molecule

     @method addAtom
     @param {String} elname Name of element
     */
    Molecule.prototype.addAtom = function (elname) {
        var self = this;

        self.atoms.push(elname);
    };

    /**
     Add a bond to the molecule

     @method addBond
     @param {Number} from Source atom
     @param {Number} to   Target atom
     */
    Molecule.prototype.addBond = function (from, to) {
        var self = this;

        self.bonds.push({from: from, to: to});
    };

    /**
     Get the InChI code for the current molecule

     @method getInChI
     @param {Function} callback Function which receives the InChI code
     @param {String}   callback.err Null if no error
     @param {String}   callback.inchi  InChI code
     */
    Molecule.prototype.getInchi = function(callback) {
        var self = this,
            mol = {
                atom: []
            };

        self.atoms.forEach(function(atom, index) {
            mol.atom.push({
                elname: atom,
                neighbor: []
            });
        });

        inchilib.GetINCHI(mol, function(err, result) {
            callback(err, result.inchi);
        });

    };

}());

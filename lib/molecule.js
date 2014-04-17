/* molecule.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
module.exports = function (inchi) {
    'use strict';

    /**
     @class Molecule
     @module InChI
     */
    var inchilib = inchi.inchilib,
        util = require('util'),
        Molecule;

    Molecule = function () {
        this.atoms = [];
        this.bonds = [];
    };

    /**
     Add an atom to the molecule

     @method addAtom
     @param {String} elname Name of element
     */
    Molecule.prototype.addAtom = function (atom) {
        var self = this;

        if (typeof atom === 'string') {
            atom = {elname: atom};
        }

        self.atoms.push(atom);
    };

    /**
     Number of atoms in this molecule

     @method atomCount
     @return {Number}
     */
    Molecule.prototype.atomCount = function () {
        var self = this;

        return self.atoms.length;
    };

    /**
     Add a bond to the molecule

     @method addBond
     @param {Number} from Source atom
     @param {Number} to   Target atom
     */
    Molecule.prototype.addBond = function (from, to, order) {
        var lastAtom = this.atoms.length - 1;

        if ((from < 0) || (from > lastAtom)) {
            throw new RangeError(util.format('addBond: index "from" (%d) out of range (0..%d)',
                                             from, lastAtom));
        }
        if ((to < 0) || (to > lastAtom)) {
            throw new RangeError(util.format('addBond: index "to" (%d) out of range (0..%d)',
                                             from, lastAtom));
        }

        order = order || 1;

        this._addBond(from, to, order);
    };

    /**
     Add a bond to the molecule (skips range-checking)

     @method _addBond
     @private
     @param {Number} from Source atom
     @param {Number} to   Target atom
     @param {Number} order Bond Order
     */
    Molecule.prototype._addBond = function (from, to, order) {
        order = order || 1;

        this.bonds.push({from: from, to: to, order: order});
    };

    /**
     Get the InChI code for the current molecule

     @method getInChI
     @param {Function} callback Function which receives the InChI code
     @param {String}   callback.err Null if no error
     @param {String}   callback.inchi  InChI code
     */
    Molecule.prototype.getInchi = function (callback) {
        var self = this,
            mol = new inchilib.Molecule(self);

        mol.GetInChI(function (err, result) {
            process.nextTick(function () {
                callback(err, result.inchi);
            });
        });
    };

    /**
     Create a molecule from an InChI code, asynchronously

     @static
     @method fromInchi
     @param {String} inchi  the InChI code
     @param {Function} callback callback function to call with completed molecule
     @param {String} callback.err  Null if no error
     @param {Molecule} callback.molecule  Null if no error
     */
    Molecule.fromInchi = function (inchi, callback) {
        inchilib.GetStructFromINCHI(inchi, function (err, struct) {
            var m = new Molecule();

            struct.atom.forEach(function (a, from) {
                m.addAtom(a);

                a.bonds.forEach(function (b) {
                    m._addBond(from, b.neighbor, b.bond_type);
                });
            });

            m.stereo0D = struct.stereo0D;

            process.nextTick(function () {
                callback(null, m);
            });
        });
    };

    return Molecule;
};

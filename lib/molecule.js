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
    Molecule.prototype.addBond = function (from, to, order) {
        var self = this;

        order = order || 1;

        self.bonds.push({from: from, to: to, order: order});
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
            callback(err, result.inchi);
        });
    };

    return Molecule;
};

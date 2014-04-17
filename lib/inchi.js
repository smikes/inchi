/* inchi.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
(function () {
    'use strict';

    var inchi = exports,
        inchilib = require('bindings')('inchi.node');

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
    inchi.getVersion = function () {
        return {
            wrapperVersion: '0.0.7',
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
    inchi.inchilib = inchilib;

    /* pull in atom */
    inchi.Atom = require('./atom')(inchi);

    /* pull in molecule */
    inchi.Molecule = require('./molecule')(inchi);

    inchi.moleculeFromMolfile = function (mol) {
        var m = new inchi.Molecule();

        mol.atoms.forEach(function (atom, i) {
            var inchiAtom = {
                x: atom.x,
                y: atom.y,
                z: atom.z,
                elname: atom.elname
            };

            if (mol.properties.CHG && mol.properties.CHG[i])
                inchiAtom.charge = mol.properties.CHG[i];
            if (mol.properties.RAD && mol.properties.RAD[i])
                inchiAtom.radical = mol.properties.RAD[i];
            if (mol.properties.ISO && mol.properties.ISO[i])
                inchiAtom.isotopic_mass = inchi.ISOTOPIC_SHIFT_FLAG + mol.properties.ISO[i];

            m.addAtom(inchiAtom);
        });

        mol.bonds.forEach(function (bond) {
            // molfile uses 1-indexed bond, but we use 0-indexed
            m.addBond(bond.from-1, bond.to-1, bond.order);
        });

        return m;
    };

    /* define some constants */
    inchi.ISOTOPIC_SHIFT_FLAG = 10000;
}());

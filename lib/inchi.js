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

    inchi.countMolfileBonds = function (mol, i) {
        return mol.bonds.filter(function (b) {
            return (b.from === i+1) || (b.to === i+1);
        }).map(function (b) {
            return b.bondType;
        }).reduce(function(acc, curr) {
            return acc + curr;
        }, 0);
    };

    inchi.moleculeFromMolfile = function (mol) {
        var m = new inchi.Molecule();

        mol.atoms.forEach(function (atom, i) {
            var inchiAtom = {
                x: atom.x,
                y: atom.y,
                z: atom.z,
                elname: atom.elname
            };

            // TODO(SOM): set num_iso_H using valence
            // num_iso_H = [n, 0, 0, 0]
            // where n == -1 if valence is not specified (this is default)
            // n = valence - #bonds if valence is specified
            if (atom.valenceCode) {
                inchiAtom.num_iso_H = [calcValence(atom.valenceCode, i), 0, 0, 0];
            }

            m.addAtom(inchiAtom);
        });

        function calcValence(code, i) {
            // find bonds to (i+1), sum order
            return code - inchi.countMolfileBonds(mol, i);
        }

        mol.bonds.forEach(function (bond) {
            // molfile uses 1-indexed atoms, but we use 0-indexed
            m.addBond(bond.from-1, bond.to-1, bond.bondType);
        });

        function setProperty(structName, inchiName, setter) {
            var pObj = mol.properties[structName];

            if (!pObj) {
                return;
            }

            Object.keys(pObj).forEach(function (k) {
                setter(m.atoms[k-1], inchiName, pObj[k]);
            });
        }

        function defSetter(molAtom, key, value) {
            molAtom[key] = value;
        }

        function isoSetter(molAtom, key, value) {
            molAtom[key] = value;
        }

        setProperty('CHG', 'charge', defSetter);
        setProperty('RAD', 'radical', defSetter);
        setProperty('ISO', 'isotopic_mass', isoSetter);

        return m;
    };

    /* define some constants */
    inchi.ISOTOPIC_SHIFT_FLAG = 10000;
}());

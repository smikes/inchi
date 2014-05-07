/* inchi.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
(function () {
    'use strict';

    var inchi = exports,
        inchilib = require('bindings')('inchi.node'),
        inner_GetStructFromINCHI = inchilib.GetStructFromINCHI,
        inner_GetINCHI = inchilib.GetINCHI;

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
            wrapperVersion: '1.0.2',
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

    /**
     @method buildError
     @private
     @return {Object} Error object or null if no error
     */
    inchi.buildError = function buildError(struct) {
        switch (struct.result) {

        case inchilib.inchi_Ret_EOF:
            return new Error('EOF - no structure found');
        }

        return null;
    };

    inchi.inchilib.GetStructFromINCHI = function GetStructFromINCHI_wrap(i, callback) {
        // TODO(SOM) check for input validity
        inner_GetStructFromINCHI(i, function (err, struct) {
            err = inchi.buildError(struct);

            callback(err, struct);
        });
    };

    function missingArray(obj, name) {
        if (obj.hasOwnProperty(name) && (obj[name] instanceof Array)) {
            return false;
        }
        return true;
    }

    function missingString(obj, name) {
        if (obj.hasOwnProperty(name) && (typeof (obj[name]) === "string")) {
            return false;
        }
        return true;
    }

    function checkValidAtom(atom, i) {
        if (!(atom instanceof Object)) {
            throw new TypeError("Expected object: atoms[" + i + "]: " + JSON.stringify(atom));
        }

        if (missingString(atom, "elname")) {
            throw new TypeError("Missing elname: atoms[" + i + "]: " + JSON.stringify(atom));
        }
    }

    function checkValidBond(length) {
        return function (bond, i) {
            if (!(bond instanceof Object)) {
                throw new TypeError("Expected object: bonds[" + i + "]: " + JSON.stringify(bond));
            }
            if (bond.from < 0 || bond.from > length) {
                throw new TypeError("Bond 'from' out of range 0.." + length +
                                    " in bonds[" + i + "] : " + JSON.stringify(bond));
            }
            if (bond.to < 0 || bond.to > length) {
                throw new TypeError("Bond 'to' out of range 0.." + length +
                                    " in bonds[" + i + "] : " + JSON.stringify(bond));
            }
        };
    }

    inchi.checkValidStruct = function checkValidStruct(mol) {
        if (!(mol instanceof Object)) {
            throw new TypeError("Expected object: " + JSON.stringify(mol));
        }

        if (missingArray(mol, "atoms")) {
            throw new TypeError("Missing array: atoms");
        }
        mol.atoms.forEach(checkValidAtom);

        if (!missingArray(mol, "bonds")) {
            mol.bonds.forEach(checkValidBond(mol.atoms.length));
        }
    };

    inchi.inchilib.GetINCHI = function GetINCHI_wrap(mol, callback) {
        try {
            inchi.checkValidStruct(mol);
        } catch (err) {
            process.nextTick(function () {
                callback(err);
            });
            return;
        }

        inner_GetINCHI(mol, function (err, result) {
            // TODO(SOM) build a new error

            callback(err, result);
        });
    };
    inchi.inchilib.GetInChIFromMolecule = inchi.inchilib.GetINCHI;

    /* pull in molfile functions */
    inchi.molfile = require('./molfileToInchi')(inchi);

    /* expose moleculeFromMolfile */
    inchi.moleculeFromMolfile = inchi.molfile.moleculeFromMolfile;

    /* define some constants */
    inchi.ISOTOPIC_SHIFT_FLAG = 10000;
}());

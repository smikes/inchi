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
            wrapperVersion: '1.0.1',
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

    inchi.inchilib.GetINCHI = function GetINCHI_wrap(struct, callback) {
        // TODO(SOM) check for input validity

        inner_GetINCHI(struct, function (err, result) {
            // TODO(SOM) build a new error

            callback(err, result);
        });

    };

    /* pull in molfile functions */
    inchi.molfile = require('./molfileToInchi')(inchi);

    /* expose moleculeFromMolfile */
    inchi.moleculeFromMolfile = inchi.molfile.moleculeFromMolfile;

    /* define some constants */
    inchi.ISOTOPIC_SHIFT_FLAG = 10000;
}());

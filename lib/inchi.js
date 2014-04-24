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
            wrapperVersion: '1.0.0',
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

    /* pull in molecule */
    inchi.Molecule = require('./molecule')(inchi);

    /* pull in molfile functions */
    inchi.molfile = require('./molfileToInchi')(inchi);

    /* expose moleculeFromMolfile */
    inchi.moleculeFromMolfile = inchi.molfile.moleculeFromMolfile;

    /* define some constants */
    inchi.ISOTOPIC_SHIFT_FLAG = 10000;
}());

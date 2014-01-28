/* javascript wrapper for inchi */

(function () {
    'use strict';

    var inchilib = require('bindings')('inchi.node');

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


}());

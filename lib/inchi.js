/* javascript wrapper for inchi */

(function () {
    'use strict';

    var inchilib = require('../build/Release/inchi.node');
    exports.inchilib = inchilib;

    /**
     * High-level javascript wrapper of InChI API
     *
     * @module InChI
     * @class  InChI
     */

    /**
     * @method version
     * @return {Object} Object containing versions of InCHI algorithm,
     * InChI library, and javascript wrapper library
     */
    exports.version = function () {
        return {
            wrapperVersion: '0.0.3pre',
            algorithmVersion: inchilib.getAlgorithmVersion(),
            libraryVersion: '1.04'
        };
    };

}());

/* javascript wrapper for inchi */

(function () {
    'use strict';

    var inchilib = require('../build/Release/libinchi.node');
    exports.inchilib = inchilib;

    /**
     * High-level javascript wrapper of InChI API
     * 
     * @module InChI
     * @class  InChI
     */

    
    exports.version = function () {
        return {
            wrapperVersion: '0.0.2pre',
            algorithmVersion: inchilib.getAlgorithmVersion(),
            libraryVersion: '1.04'
        };
    };

}());

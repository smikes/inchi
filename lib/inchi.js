/* javascript wrapper for inchi */

(function () {
    'use strict';

    var inchiLib = require('../build/Release/libinchi.node');
    
    /**
     * this is the higher-level javascript wrapper
     * 
     * @module InChI
     * @class  InChI
     */

    
    exports.version = function () {
        return {
            wrapperVersion: '0.0.2pre',
            algorithmVersion: inchiLib.getAlgorithmVersion(),
            libraryVersion: '1.04'
        };
    };

}());

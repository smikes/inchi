/* javascript wrapper for inchi wrapper */

(function () {
    'use strict';

    var inchiLib = require('../build/Release/libinchi.node');
    
    // publish something to exports
    exports.version = function () {
        return {
            wrapperVersion: '0.0.1',
            algorithmVersion: inchiLib.getAlgorithmVersion(),
            libraryVersion: '1.04'
        };
    };

}());

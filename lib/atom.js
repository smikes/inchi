/* atom.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
module.exports = function (inchi) {
    'use strict';
    var inchilib = inchi.inchilib,
        Atom;

    Atom = function () {
        var self = this;
        self._atom = new inchilib.Atom();
    };

    return Atom;
};

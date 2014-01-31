/* atom.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
module.exports = function (inchi) {
    'use strict';
    var Atom;

    Atom = function (name) {
        this.elname = name;
    };
    Atom.prototype.getName = function () {
        return this.elname;
    };
    Atom.prototype.setName = function (name) {
        this.elname = name;
    };

    return Atom;
};

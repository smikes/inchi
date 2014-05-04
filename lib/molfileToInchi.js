/* inchi.js
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
module.exports = function (inchi) {
    'use strict';

    var molfileToInchi = {};

    var Molecule = require('ichem-molecule');

    function countMolfileBonds(mol, i) {
        return mol.bonds.filter(function findBond(b) {
            return (b.from === i + 1) || (b.to === i + 1);
        }).map(function getOrder(b) {
            return b.bondType;
        }).reduce(function sum(acc, curr) {
            return acc + curr;
        }, 0);
    }
    molfileToInchi.countMolfileBonds = countMolfileBonds;

    // TODO(SOM): set num_iso_H using valence
    // num_iso_H = [n, 0, 0, 0]
    // where n == -1 if valence is not specified (this is default)
    // n = valence - #bonds if valence is specified
    function makeNumIsoH(code, bonds) {
        var valence;

        if (code === 15) {
            valence = 0;
        } else if (code === 0) {
            valence = -1;
        } else {
            // num_iso_H should never be negative when valence is set
            valence = Math.max(0, code - bonds);
        }

        return [valence, 0, 0, 0];
    }
    molfileToInchi.makeNumIsoH = makeNumIsoH;

    function setProperty(mol, m, structName, inchiName) {
        var pObj = mol.properties[structName];

        if (!pObj) {
            return;
        }

        Object.keys(pObj).forEach(function (k) {
            m.atoms[k - 1][inchiName] = pObj[k];
        });
    }

    function atomPropertiesFromMolfile(mol, m) {
        var propertyTable = {
            CHG: 'charge',
            RAD: 'radical',
            ISO: 'isotopic_mass'
        };

        Object.keys(propertyTable).forEach(function (molName) {
            var inchiName = propertyTable[molName];

            setProperty(mol, m, molName, inchiName);
        });
    }

    molfileToInchi.moleculeFromMolfile = function (mol) {
        var m = new Molecule();

        mol.atoms.forEach(function (atom, i) {
            var inchiAtom = {
                x: atom.x,
                y: atom.y,
                z: atom.z,
                elname: atom.elname
            };

            inchiAtom.num_iso_H = makeNumIsoH(atom.valenceCode,
                                              countMolfileBonds(mol, i));

            m.addAtom(inchiAtom);
        });

        mol.bonds.forEach(function (bond) {
            // molfile uses 1-indexed atoms, but we use 0-indexed
            m.addBond(bond.from - 1, bond.to - 1, bond.bondType, bond.bondStereo);
        });

        atomPropertiesFromMolfile(mol, m);

        return m;
    };

    return molfileToInchi;
};

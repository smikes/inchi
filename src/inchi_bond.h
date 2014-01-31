#ifndef SRC_INCHI_BOND_H_
#define SRC_INCHI_BOND_H_
/* inchi_bond.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/**
   Internal class for representing a two-atom bond

   @class InchiBond
   @module Internal
 */

struct InchiBond {
  /**
     Constructs a bond from one atom to another , with optional bond order and stereochemistry.

     @constructor
     @method InchiBond
     @param {int} from  Atom index at beginning of bond
     @param {int} to    Atom index at end of bond
     @param {inchi_BondType} [type=INCHI_BOND_TYPE_SINGLE]  Bond type -- actually bond order,
     with 4 representing 'alternative'
     @param {inchi_BondStereo2D} [stereo=INCHI_BOND_STEREO_NONE]  Bond stereochemistry -- only
     2D stereochemistry options available. TODO(SOM): currently has no effect on bond
   */
  InchiBond(int from, int to, int type = INCHI_BOND_TYPE_SINGLE)
  : from(from), to(to),
    type(type), stereo(INCHI_BOND_STEREO_NONE) {
  }

  int from;

  int to;

  S_CHAR type;

  S_CHAR stereo;
};

#endif  // SRC_INCHI_BOND_H_

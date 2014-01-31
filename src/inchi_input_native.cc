/* inchi_input_native.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./molecule.h"

#include <cstring>
#include <algorithm>

#include "inchi_dll/inchi_api.h"

/**
@module Internal
@class InchiInput
 */

/**
 * Default constructor initializes everything to zero
 *
 * @method InchiInput
 * @constructor
 */
InchiInput::InchiInput() {
}

/**
 * Destructor: cleans up InchiInput
 *
 * @method ~InchiInput
 */
InchiInput::~InchiInput() {
}


/**
 * adds an atom to the molecule
 *
 * @method addAtom
 * @param {const char *} elementName
 * @return {int} atom index
 */
int InchiInput::addAtom(const char * elementName) {
  InchiAtom a(elementName);

  atoms_.push_back(a);

  return atoms_.size() - 1;
}

/**
 * Get a modifiable reference to an atom
 *
 * @method getAtom
 * @param {int} atomIndex 0-based index into atom collection
 * @return {InchiAtom& atom}
 */
InchiAtom& InchiInput::getAtom(int atomIndex) {
  // TODO(SOM): bounds checking

  return atoms_[atomIndex];
}

/**
 * Add a bond to the molecule
 *
 * @method addBond
 * @param {InchiBond} bond   A bond between two atoms
 */
void InchiInput::addBond(const InchiBond& bond) {
  bonds_.push_back(bond);
}


/**
 * Helper class for converting bonds into neighbor matrix entries
 *
 * @module Internal
 * @class make_bond
 */
struct make_bond {
  inchi_Input * in;

  explicit make_bond(GetINCHIData * data) :
    in(&(data->in_)) {}

  /**
   * converts an InchiBond to a neighbor matrix entry
   *
   * @method operator()
   * @param {const InchiBond&} bond   InchiBond being added to molecule
   */
  void operator()(const InchiBond& bond) {
    int from = bond.from;

    AT_NUM& num_bonds = in->atom[from].num_bonds;

    in->atom[from].neighbor[num_bonds]    = bond.to;
    in->atom[from].bond_type[num_bonds]   = bond.type;
    in->atom[from].bond_stereo[num_bonds] = bond.stereo;

    num_bonds += 1;
  }
};

/**
 * Makes a copy of the molecule data, suitable for calling GetINCHI.
 * This copy is not visible from JavaScript, which makes it usable
 * on a thread (see {{#crossLink "InChILib/GetINCHI"}}GetInChI{{/crossLink}})
 *
 * @method tearOffGetINCHIData
 * @for InchiInput
 */
GetINCHIData * InchiInput::tearOffGetINCHIData() {
  GetINCHIData * data = new GetINCHIData();

  data->in_.num_atoms = atoms_.size();
  data->in_.atom = new inchi_Atom[data->in_.num_atoms];

  std::copy(atoms_.begin(), atoms_.end(), data->in_.atom);

  std::for_each(bonds_.begin(), bonds_.end(), make_bond(data));

  return data;
}

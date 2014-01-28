/* inchi_input_native.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_input.h"

#include <cstring>
#include <algorithm>

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
 * returns a modifiable reference to an atom
 *
 * @method getAtom
 * @param {int} atomIndex 0-based index into atom collection
 * @return {InchiAtom&} atom
 */
InchiAtom& InchiInput::getAtom(int atomIndex) {
  // TODO(SOM): bounds checking

  return atoms_[atomIndex];
}

void InchiInput::addBond(const InchiBond& b) {
  bonds_.push_back(b);
}

struct make_bond {
  struct GetINCHIData * data_;

  explicit make_bond(struct GetINCHIData * data) :
    data_(data) {}

  void operator()(const InchiBond& b) {
    int from = b.from;

    AT_NUM& num_bonds = data_->in_.atom[from].num_bonds;

    data_->in_.atom[from].neighbor[num_bonds]    = b.to;
    data_->in_.atom[from].bond_type[num_bonds]   = b.type;
    data_->in_.atom[from].bond_stereo[num_bonds] = b.stereo;

    num_bonds++;
  }
};

/**
 *
 *
 * @method tearOffGetINCHIData
 *
 */
GetINCHIData * InchiInput::tearOffGetINCHIData() {
  GetINCHIData * data = new GetINCHIData();

  data->in_.num_atoms = atoms_.size();
  data->in_.atom = new inchi_Atom[data->in_.num_atoms];

  std::copy(atoms_.begin(), atoms_.end(), data->in_.atom);

  std::for_each(bonds_.begin(), bonds_.end(), make_bond(data));

  return data;
}

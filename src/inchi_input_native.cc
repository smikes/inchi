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

void InchiInput::addBond(int f, int t) {
  bonds_.push_back(InchiBond(f, t));
}

struct make_bond {
  struct InchiInput::GetINCHIData * data_;

  explicit make_bond(struct InchiInput::GetINCHIData * data) :
    data_(data) {}

  void operator()(const InchiInput::InchiBond& b) {
    int from = b.from;

    AT_NUM& num_bonds = data_->in_.atom[from].num_bonds;

    data_->in_.atom[from].neighbor[num_bonds]    = b.to;
    data_->in_.atom[from].bond_type[num_bonds]   = INCHI_BOND_TYPE_SINGLE;
    data_->in_.atom[from].bond_stereo[num_bonds] = INCHI_BOND_STEREO_NONE;

    num_bonds++;
  }
};

/**
 *
 *
 * @method tearOffGetINCHIData
 *
 */
InchiInput::GetINCHIData * InchiInput::tearOffGetINCHIData() {
  GetINCHIData * data = new GetINCHIData();

  data->in_.num_atoms = atoms_.size();
  data->in_.atom = new inchi_Atom[data->in_.num_atoms];

  std::copy(atoms_.begin(), atoms_.end(), data->in_.atom);

  std::for_each(bonds_.begin(), bonds_.end(), make_bond(data));

  return data;
}

/**
 *
 * @class GetINCHIData
 */


/**
 * calculate INCHI from structure
 *
 * @method GetInchi
 * @return {RetValGetInchi} result code from GetINCHI API call
 */
int InchiInput::GetINCHIData::GetInchi() {
  this->result_ = GetINCHI(&(this->in_), &(this->out_));

  return this->result_;
}


InchiInput::GetINCHIData::GetINCHIData() {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
}

InchiInput::GetINCHIData::~GetINCHIData() {
  FreeINCHI(&out_);
  delete in_.atom;
  delete in_.stereo0D;
}

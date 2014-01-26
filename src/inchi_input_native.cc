/* inchi_input_native.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_input.h"


/**
 * Default constructor initializes everything to zero
 *
 * @method InchiInput
 * @constructor
 */
InchiInput::InchiInput() {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
}

/**
 * Destructor: cleans up InchiInput
 *
 * @method ~InchiInput
 */
InchiInput::~InchiInput() {
  FreeINCHI(&out_);
  delete in_.atom;
  delete in_.stereo0D;
}

/**
 * adds an atom to the molecule
 *
 * @method addAtom
 * @param {const char *} elementName
 * @return {int} atom index
 */
int InchiInput::addAtom(const char * elementName) {
  InchiAtom a;

  strncpy(a.elname, elementName, ATOM_EL_LEN);
  a.elname[ATOM_EL_LEN - 1] = '\0';

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

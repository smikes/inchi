/* inchi_input_native.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./molecule.h"

#include <algorithm>

#include "inchi_dll/inchi_api.h"

#include "./get_struct_from_inchi_data.h"

/**
@module Internal
@class Molecule_CC
 */

/**
 * Default constructor initializes everything to zero
 *
 * @method Molecule
 * @constructor
 */
Molecule::Molecule() {
}

/**
 * Destructor: cleans up Molecule
 *
 * @method ~Molecule
 */
Molecule::~Molecule() {
}

/**
 * adds an atom to the molecule
 *
 * @method addAtom
 * @param {const InchiAtom&} atom
 * @return {int} atom index
 */
int Molecule::addAtom(const InchiAtom& atom) {
  InchiAtom a(atom);

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
InchiAtom& Molecule::getAtom(int atomIndex) {
  // TODO(SOM): bounds checking

  return atoms_[atomIndex];
}

/**
 Number of atoms in molecule

 @method getAtomCount
 @return {AT_NUM} number of atoms
*/
AT_NUM Molecule::getAtomCount() {
  return atoms_.size();
}


/**
 * Add a bond to the molecule
 *
 * @method addBond
 * @param {InchiBond} bond   A bond between two atoms
 */
void Molecule::addBond(const InchiBond& bond) {
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

    // clamp array index to legitimate values
    if (from < 0) {
      from = 0;
    } else if (from >= in->num_atoms) {
      from = in->num_atoms - 1;
    }

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
 * @for Molecule_CC
 */
GetINCHIData * Molecule::tearOffGetINCHIData() {
  GetINCHIData * data = new GetINCHIData();

  data->in_.num_atoms = atoms_.size();
  data->in_.atom = new inchi_Atom[data->in_.num_atoms];

  data->in_.num_stereo0D = stereo0D_.size();
  data->in_.stereo0D = new inchi_Stereo0D[data->in_.num_stereo0D];

  std::copy(atoms_.begin(), atoms_.end(), data->in_.atom);

  std::for_each(bonds_.begin(), bonds_.end(), make_bond(data));

  std::copy(stereo0D_.begin(), stereo0D_.end(), data->in_.stereo0D);

  return data;
}

void Molecule::addStereo(const InchiStereo& s) {
  stereo0D_.push_back(s);
}

Molecule * Molecule::fromInchi(const std::string& inchi) {
  GetStructFromINCHIData data(inchi);

  int result = data.GetStructFromINCHI();

  if (result != inchi_Ret_OKAY) {
    return NULL;
  }

  Molecule * m = new Molecule;

  for (int i = 0; i < data.out_.num_atoms; i += 1) {
    m->addAtom(InchiAtom(data.out_.atom[i]));
  }

  for (int i = 0; i < data.out_.num_stereo0D; i += 1) {
    m->addStereo(InchiStereo(data.out_.stereo0D[i]));
  }

  return m;
}

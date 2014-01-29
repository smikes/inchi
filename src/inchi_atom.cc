/* inchi_atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_atom.h"

#include <string>
#include <cstring>

/**
   Internal representation of an atom; convertible to inchi_Atom

   @module Internal
   @class  InchiAtom
 */

/**
 * Default constructor
 *
 * @method InchiAtom
 * @param {const char *} [name=""] element name
 */
InchiAtom::InchiAtom(const char * name) {
  this->setName(name);
  this->num_iso_H[NON_ISOTOPIC_H] = IMPLICIT_NONISOTOPIC_H;
  this->num_iso_H[PROTIUM_H] = 0;
  this->num_iso_H[DEUTERIUM_H] = 0;
  this->num_iso_H[TRITIUM_H] = 0;
}

/**
 * Set the atom's element name
 *
 * @method setName
 * @param {const char *} name element name
 */
void InchiAtom::setName(const char * name) {
  strncpy(this->elname, name, ELNAME_LEN);
  this->elname[ELNAME_LEN - 1] = '\0';
}

/**
   Get the atom's element name
   @method getName
   @return {const std::string} element name
 */
const std::string InchiAtom::getName() {
  return elname;
}

/**
   Conversion (cast operator) to an inchi_Atom structure

   @method operator inchi_Atom()
   @return {inchi_Atom}
 */
InchiAtom::operator inchi_Atom() {
  inchi_Atom a;
  memset(&a, 0, sizeof inchi_Atom);

  memcpy(a.elname, this->elname, ELNAME_LEN);
  memcpy(a.num_iso_H, this->num_iso_H, NUM_H_ISOTOPES+1);

  // TODO(SOM): populate remaining members, excluding
  // neighbor, bond_type, bond_stereo, num_bonds
  // which are populated via bonds_

  return a;
}

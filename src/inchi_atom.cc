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
  memset(&(this->data_), 0, sizeof(this->data_));
  this->setName(name);
  this->data_.num_iso_H[NON_ISOTOPIC_H] = IMPLICIT_NONISOTOPIC_H;
  this->data_.num_iso_H[PROTIUM_H] = 0;
  this->data_.num_iso_H[DEUTERIUM_H] = 0;
  this->data_.num_iso_H[TRITIUM_H] = 0;
}

/**
 * Set the atom's element name
 *
 * @method setName
 * @param {const char *} name element name
 */
void InchiAtom::setName(const char * name) {
  strncpy(this->data_.elname, name, ELNAME_LEN);
  this->data_.elname[ELNAME_LEN - 1] = '\0';
}

/**
   Get the atom's element name
   @method getName
   @return {const std::string} element name
 */
const std::string InchiAtom::getName() {
  return data_.elname;
}

/**
   Conversion (cast operator) to an inchi_Atom structure

   @method operator inchi_Atom()
   @return {inchi_Atom}
 */
InchiAtom::operator inchi_Atom() {
  inchi_Atom a(this->data_);

  return a;
}

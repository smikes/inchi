/* inchi_atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_atom.h"

#include <string>
#include <cstring>

/**
 * Internal InchiAtom structure
 *
 * @module InChILib
 * @class  InchiAtom
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
 * Get the atom's element name
 *
 * @method getName
 * @return {const std::string} element name
 */
const std::string InchiAtom::getName() {
  return elname;
}

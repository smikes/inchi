/* inchi_atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_atom.h"

#include <string>

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
  strncpy(this->elname, name, ELNAME_LEN);
  this->elname[ELNAME_LEN - 1] = '\0';
}

/**
 * get the atom name
 *
 * @method getName
 * @return {const std::string} atom name
 */
const std::string InchiAtom::getName() {
  return elname;
}

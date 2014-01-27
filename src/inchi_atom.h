#ifndef SRC_INCHI_ATOM_H_
#define SRC_INCHI_ATOM_H_
/* inchi_atom.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <string>

#include "inchi_dll/inchi_api.h"

struct InchiAtom {
  static const int ELNAME_LEN = ATOM_EL_LEN;

  /* data members */
  char elname[ELNAME_LEN];

  /* constructors */
  explicit InchiAtom(const char * name = "");

  /* accessors */
  const std::string getName();
  void setName(const char *name);
};

#endif  // SRC_INCHI_ATOM_H_

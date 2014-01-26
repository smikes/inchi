#ifndef SRC_INCHI_ATOM_H_
#define SRC_INCHI_ATOM_H_
/* inchi_atom.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "inchi_dll/inchi_api.h"

struct InchiAtom {
  static const int ELNAME_LEN = ATOM_EL_LEN;

  char elname[ELNAME_LEN];
};

#endif  // SRC_INCHI_ATOM_H_

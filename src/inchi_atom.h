#ifndef SRC_INCHI_ATOM_H_
#define SRC_INCHI_ATOM_H_
/* inchi_atom.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>

#include <string>

#include "inchi_dll/inchi_api.h"

struct InchiAtom {
  static const int     ELNAME_LEN = ATOM_EL_LEN;
  static const S_CHAR  IMPLICIT_NONISOTOPIC_H = -1;

  enum { NON_ISOTOPIC_H = 0,
         PROTIUM_H      = 1,
         DEUTERIUM_H    = 2,
         TRITIUM_H      = 3 };

  /* data members */
  inchi_Atom data_;

  /* constructors */
  explicit InchiAtom(const char * name = "");
  explicit InchiAtom(const inchi_Atom& atom) : data_(atom) {
  }

  /* accessors */
  const std::string getName();
  void setName(const char *name);

  operator inchi_Atom();
};

#endif  // SRC_INCHI_ATOM_H_

#ifndef SRC_MOLECULE_H_
#define SRC_MOLECULE_H_
/* molecule.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <node.h>
#include <v8.h>

#include <vector>

#include "./using_v8.h"

#include "inchi_dll/inchi_api.h"

#include "./inchi_atom.h"
#include "./inchi_bond.h"
#include "./inchi_stereo.h"
#include "./get_inchi_data.h"

struct Molecule {
  Molecule();
  ~Molecule();

  std::vector<InchiAtom> atoms_;
  std::vector<InchiBond> bonds_;
  std::vector<InchiStereo> stereo0D_;

  /* native api */
  int addAtom(const char * element);
  InchiAtom& getAtom(int i);

  AT_NUM getAtomCount();

  void addBond(const InchiBond& b);

  void addStereo(const InchiStereo& s);

  GetINCHIData * tearOffGetINCHIData();

  static Molecule * fromInchi(const char * inchi);

  /* javascript api */
  static Molecule * Create(Handle<Value> val);
};

Handle<Object> GetResult(GetINCHIData * data);

#endif  // SRC_MOLECULE_H_

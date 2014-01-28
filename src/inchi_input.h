#ifndef SRC_INCHI_INPUT_H_
#define SRC_INCHI_INPUT_H_
/* inchi_input.h
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

struct InchiInput {
  InchiInput();
  ~InchiInput();

  struct GetINCHIData {
    GetINCHIData();
    ~GetINCHIData();

    inchi_Input in_;
    inchi_Output out_;
    int result_;

    int GetInchi();
    Handle<Object> GetResult();
  };

  struct InchiBond {
    InchiBond(int f, int t) : from(f), to(t) {}

    int from;
    int to;
  };

  std::vector<InchiAtom> atoms_;
  std::vector<InchiBond> bonds_;
  std::vector<inchi_Stereo0D> stereo0D_;

  /* native api */
  int GetInchi();

  int addAtom(const char * element);
  InchiAtom& getAtom(int i);

  void addBond(int f, int t);

  GetINCHIData * tearOffGetINCHIData();

  static InchiInput * Create(Handle<Value> val);
};

#endif  // SRC_INCHI_INPUT_H_

#ifndef SRC_INCHI_INPUT_H_
#define SRC_INCHI_INPUT_H_
/* inchi_input.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <vector>

#include "node.h"
#include "v8.h"

#include "./using_v8.h"

#include "inchi_dll/inchi_api.h"

#include "./inchi_atom.h"

struct InchiInput {
  InchiInput();
  ~InchiInput();

  inchi_Input in_;
  inchi_Output out_;

  int result_;

  std::vector<InchiAtom> atoms_;
  std::vector<inchi_Stereo0D> stereo0D_;

  /* native api */
  int addAtom(const char * element);
  int GetInchi();

  InchiAtom& getAtom(int i);

  static InchiInput * Create(Handle<Value> val);

  Handle<Object> GetResult();
};

#endif  // SRC_INCHI_INPUT_H_

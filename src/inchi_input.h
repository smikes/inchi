#ifndef SRC_INCHI_INPUT_H_
#define SRC_INCHI_INPUT_H_
/* inchi_input.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <vector>

#include <node.h>
#include <v8.h>

#include "./using_v8.h"

#include "inchi_dll/inchi_api.h"

struct InchiInput {
  InchiInput();
  ~InchiInput();

  std::vector<inchi_Atom> atoms_;
  std::vector<inchi_Stereo0D> stereo0D_;

  static InchiInput * Construct(Handle<Value> val);
};

#endif  // SRC_INCHI_INPUT_H_

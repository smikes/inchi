#ifndef SRC_INCHI_INPUT_H_
#define SRC_INCHI_INPUT_H_
/* inchi_input.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "inchi_dll/inchi_api.h"

struct InchiInput : public tagINCHI_Input {
  InchiInput();
  ~InchiInput();
};

#endif  // SRC_INCHI_INPUT_H_

#include "node.h"
#include "v8.h"
#include "uv.h"
#include "nan.h"

#include "./inchi_input.h"

#include "./using_v8.h"


/**
 * Default constructor initializes everything to zero
 *
 * @method InchiInput
 * @constructor
 */
InchiInput::InchiInput() {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
}

/**
 * Destructor: cleans up InchiInput
 *
 * @method ~InchiInput
 */
InchiInput::~InchiInput() {
  FreeINCHI(&out_);
  delete in_.atom;
  delete in_.stereo0D;
}

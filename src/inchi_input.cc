/* inchi_input.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include "./inchi_input.h"

/**
 * Wrapper for INCHI_Input structure
 *
 * @module InChILib
 * @class  InchiInput
 */

/**
 * Default constructor initializes everything to zero
 *
 * @method InchiInput
 * @constructor
 */
InchiInput::InchiInput() {
  /* can't use base initialization */
  atom = 0;
  stereo0D = 0;
  szOptions = 0;
  num_atoms = 0;
  num_stereo0D = 0;
}

/**
 * Destructor: cleans up InchiInput
 *
 * @method ~InchiInput
 */
InchiInput::~InchiInput() {
}

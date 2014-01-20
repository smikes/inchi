#include "inchi_input.h"

InchiInput::InchiInput() {
  /* can't use base initialization */
  atom = 0;
  stereo0D = 0;
  szOptions = 0;
  num_atoms = 0;
  num_stereo0D = 0;
}

InchiInput::~InchiInput() {
}

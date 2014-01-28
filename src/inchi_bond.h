#ifndef SRC_INCHI_BOND_H_
#define SRC_INCHI_BOND_H_
/* inchi_input.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

struct InchiBond {
InchiBond(int f, int t, int type = INCHI_BOND_TYPE_SINGLE) :
  from(f), to(t),
    type(type),
    stereo(INCHI_BOND_STEREO_NONE) {
  }

  int from;
  int to;
  S_CHAR type;
  S_CHAR stereo;
};

#endif  // SRC_INCHI_BOND_H_

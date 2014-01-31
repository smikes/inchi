#ifndef SRC_ATOM_H_
#define SRC_ATOM_H_
/* atom.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./inchi_atom.h"

struct Atom : public node::ObjectWrap {
  Atom(const char * name = "");

  static void Init(v8::Handle<v8::Object> exports);

  static NAN_METHOD(New);
  static NAN_METHOD(GetName);
  static NAN_METHOD(SetName);

  InchiAtom atom;
};
#endif  // SRC_ATOM_H_

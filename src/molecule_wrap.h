#ifndef SRC_MOLECULE_WRAP_H_
#define SRC_MOLECULE_WRAP_H_
/* molecule_wrap.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./molecule.h"
#include "./using_v8.h"

struct Molecule_wrap : public node::ObjectWrap {
  Molecule_wrap() {}
  ~Molecule_wrap() {}

  static NAN_METHOD(New);
  static NAN_METHOD(GetInChI);

  static void Init(v8::Handle<v8::Object> exports);

  void addAtoms(Handle<Value> atoms);
  void addBonds(Handle<Value> bonds);
  void addStereo(Handle<Value> stereo);

  Molecule mol;
};

#endif  // SRC_MOLECULE_WRAP_H_

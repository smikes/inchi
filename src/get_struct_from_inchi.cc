/* get_struct_from_inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./using_v8.h"

#include "./get_struct_from_inchi_data.h"
#include "./get_struct_from_inchi_worker.h"

void addstring(Handle<Object> ret, const char * name, const char * value);

NAN_METHOD(GetStructFromINCHI) {
  NanScope();

  std::string inchi = NanCString(args[0], 0);

  NanCallback * callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new GetStructFromINCHIWorker(callback, inchi));

  NanReturnUndefined();
}

static Handle<Object> GetBond(const inchi_Atom& a, int n) {
  Local<Object> ret = Object::New();

  ret->Set(NanSymbol("neighbor"), Number::New(a.neighbor[n]));
  ret->Set(NanSymbol("bond_type"), Number::New(a.bond_type[n]));
  ret->Set(NanSymbol("bond_stereo"), Number::New(a.bond_stereo[n]));

  return ret;
}

static Handle<Object> GetAtom(const inchi_Atom& a) {
  Local<Object> ret = Object::New();

  ret->Set(NanSymbol("x"), Number::New(a.x));
  ret->Set(NanSymbol("y"), Number::New(a.y));
  ret->Set(NanSymbol("z"), Number::New(a.z));
  addstring(ret, "elname", a.elname);

  // compress neighbor, bond_type, and bond_stereo arrays
  Local<Array> bonds = Array::New();
  for (int i = 0; i < a.num_bonds; i += 1) {
    bonds->Set(Number::New(i), GetBond(a, i));
  }
  ret->Set(NanSymbol("bonds"), bonds);

  // implicit hydrogens
  Local<Array> num_iso_H = Array::New();
  for (int i = 0; i < NUM_H_ISOTOPES+1; i += 1) {
    num_iso_H->Set(Number::New(i), Number::New(a.num_iso_H[i]));
  }
  ret->Set(NanSymbol("num_iso_H"), num_iso_H);

  ret->Set(NanSymbol("isotopic_mass"), Number::New(a.isotopic_mass));
  ret->Set(NanSymbol("radical"), Number::New(a.radical));
  ret->Set(NanSymbol("charge"), Number::New(a.charge));

  return ret;
}

Handle<Object> GetRawStructure(const GetStructFromINCHIData& data) {
  Local<Object> ret = Object::New();

  // atom -- array of atom objects
  Local<Array> atom = Array::New();
  for (int i = 0; i < data.out_.num_atoms; i += 1) {
    atom->Set(Number::New(i), GetAtom(data.out_.atom[i]));
  }
  ret->Set(NanSymbol("atom"), atom);

  // stereo0D -- array of stereo0D objects

  // message
  addstring(ret, "message", data.out_.szMessage);

  // log
  addstring(ret, "log", data.out_.szLog);

  // warning flags

  return ret;
}

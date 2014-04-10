/* get_struct_from_inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./using_v8.h"

#include "./get_struct_from_inchi_data.h"
#include "./get_struct_from_inchi_worker.h"

#include "./inchi_stereo.h"


void addstring(Handle<Object> ret, const char * name, const char * value);

NAN_METHOD(GetStructFromINCHI) {
  NanScope();

  char * inchi = NanCString(args[0], 0);

  NanCallback * callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new GetStructFromINCHIWorker(callback, inchi));

  delete[] inchi;

  NanReturnUndefined();
}

static Handle<Object> MakeBond(const inchi_Atom& a, int n) {
  Local<Object> ret = Object::New();

  ret->Set(NanSymbol("neighbor"), Number::New(a.neighbor[n]));
  ret->Set(NanSymbol("bond_type"), Number::New(a.bond_type[n]));
  ret->Set(NanSymbol("bond_stereo"), Number::New(a.bond_stereo[n]));

  return ret;
}

static Handle<Object> MakeAtom(const inchi_Atom& a) {
  Local<Object> ret = Object::New();

  ret->Set(NanSymbol("x"), Number::New(a.x));
  ret->Set(NanSymbol("y"), Number::New(a.y));
  ret->Set(NanSymbol("z"), Number::New(a.z));
  addstring(ret, "elname", a.elname);

  // compress neighbor, bond_type, and bond_stereo arrays
  Local<Array> bonds = Array::New();
  for (int i = 0; i < a.num_bonds; i += 1) {
    bonds->Set(i, MakeBond(a, i));
  }
  ret->Set(NanSymbol("bonds"), bonds);

  // implicit hydrogens
  Local<Array> num_iso_H = Array::New();
  for (int i = 0; i < NUM_H_ISOTOPES+1; i += 1) {
    num_iso_H->Set(i, Number::New(a.num_iso_H[i]));
  }
  ret->Set(NanSymbol("num_iso_H"), num_iso_H);

  ret->Set(NanSymbol("isotopic_mass"), Number::New(a.isotopic_mass));
  ret->Set(NanSymbol("radical"), Number::New(a.radical));
  ret->Set(NanSymbol("charge"), Number::New(a.charge));

  return ret;
}

Handle<Object> MakeStereo0D(const inchi_Stereo0D& stereo) {
  Local<Object> ret = Object::New();

  Local<Array> neighbor = Array::New();
  for (int i = 0; i < STEREO0D_NEIGHBORS; i += 1) {
    neighbor->Set(i, Number::New(stereo.neighbor[i]));
  }
  ret->Set(NanSymbol("neighbor"), neighbor);

  ret->Set(NanSymbol("central_atom"), Number::New(stereo.central_atom));
  ret->Set(NanSymbol("type"), Number::New(stereo.type));
  ret->Set(NanSymbol("parity"), Number::New(stereo.parity));

  return ret;
}

Handle<Object> MakeStructure(const GetStructFromINCHIData& data) {
  Local<Object> ret = Object::New();

  // atom -- array of atom objects
  Local<Array> atom = Array::New();
  for (int i = 0; i < data.out_.num_atoms; i += 1) {
    atom->Set(i, MakeAtom(data.out_.atom[i]));
  }
  ret->Set(NanSymbol("atom"), atom);

  // stereo0D -- array of stereo0D objects
  Local<Array> stereo0D = Array::New();
  for (int i = 0; i < data.out_.num_stereo0D; i += 1) {
    stereo0D->Set(i, MakeStereo0D(data.out_.stereo0D[i]));
  }
  ret->Set(NanSymbol("stereo0D"), stereo0D);

  // message
  addstring(ret, "message", data.out_.szMessage);

  // log
  addstring(ret, "log", data.out_.szLog);

  // warning flags
  // TODO(SOM): add these

  return ret;
}

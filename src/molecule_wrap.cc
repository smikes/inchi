/* molecule_wrap.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "./molecule_wrap.h"

#include "./using_v8.h"

#include "./get_inchi.h"
#include "./get_inchi_data.h"
#include "./get_inchi_worker.h"

#include "./inchi_queue.h"

#include "./node_inchi.h"

NAN_METHOD(GetINCHI) {
  NanScope();

  Handle<Object> in = args[0]->ToObject();
  NanCallback * callback = new NanCallback(args[1].As<Function>());

  Molecule_wrap * m = new Molecule_wrap;

  m->addAtoms(in->Get(NanSymbol("atoms")));

  m->addBonds(in->Get(NanSymbol("bonds")));

  m->addStereo(in->Get(NanSymbol("stereo0D")));

  Enqueue(new GetINCHIWorker(callback, &(m->mol)));

  NanReturnUndefined();
}

void Molecule_wrap::addAtoms(Handle<Value> a) {
  Handle<Array> atoms = a.As<Array>();

  // atoms is an array of v8::Object
  for (uint32_t i = 0; i < atoms->Length(); i += 1) {
    Handle<Object> atom = atoms->Get(i)->ToObject();

    mol.addAtom(InchiAtom_makeFromObject(atom));
  }
}

template<typename T> T getIf(Handle<Object> atom, const char * k) {
  Handle<String> key = NanSymbol(k);
  return atom->Has(key) ? T(atom->Get(key)->NumberValue()) : 0;
}

template<typename T> T getIf(Handle<Object> atom, uint32_t key) {
  return atom->Has(key) ? T(atom->Get(key)->NumberValue()) : 0;
}

const InchiAtom InchiAtom_makeFromObject(Handle<Object> atom) {
  char * elname = NanCString(atom->Get(NanSymbol("elname")), 0);
  InchiAtom ret(elname);
  delete[] elname;

  ret.data_.x = getIf<double>(atom, "x");
  ret.data_.y = getIf<double>(atom, "y");
  ret.data_.z = getIf<double>(atom, "z");

  // ignore bond information; stored separately

  Handle<Array> num_iso_H = atom->Get(NanSymbol("num_iso_H")).As<Array>();
  if (!num_iso_H->IsNull() && !num_iso_H->IsUndefined()) {
    for (int i = 0; i < (NUM_H_ISOTOPES+1); i += 1) {
      ret.data_.num_iso_H[i] = getIf<S_CHAR>(num_iso_H, i);
    }
  }

  ret.data_.isotopic_mass = getIf<AT_NUM>(atom, "isotopic_mass");
  ret.data_.radical = getIf<S_CHAR>(atom, "radical");
  ret.data_.charge = getIf<S_CHAR>(atom, "charge");

  return ret;
}

void Molecule_wrap::addBonds(Handle<Value> b) {
  Handle<Array> bonds = b.As<Array>();

  // bonds is an array of v8::Object
  for (uint32_t i = 0; i < bonds->Length(); i += 1) {
    Handle<Object> bond = bonds->Get(i)->ToObject();

    int from  = bond->Get(NanSymbol("from"))->NumberValue();
    int to    = bond->Get(NanSymbol("to"))->NumberValue();
    int order = bond->Get(NanSymbol("order"))->NumberValue();

    InchiBond b = (order ? InchiBond(from, to, order) : InchiBond(from, to));

    if (bond->Has(NanSymbol("stereo"))) {
      int stereo = bond->Get(NanSymbol("stereo"))->NumberValue();
      b.stereo = stereo;
    }

    mol.addBond(b);
  }
}

void Molecule_wrap::addStereo(Handle<Value> s) {
  if (s->IsNull() || s->IsUndefined()) {
    return;
  }

  Handle<Array> stereos = s.As<Array>();

  for (uint32_t i = 0; i < stereos->Length(); i += 1) {
    Handle<Object> stereo = stereos->Get(i)->ToObject();

    mol.addStereo(InchiStereo_makeFromObject(stereo));
  }
}

const InchiStereo InchiStereo_makeFromObject(Handle<Object> stereo) {
  InchiStereo ret;

  Local<Array> neighbors = stereo->Get(NanSymbol("neighbor")).As<Array>();
  for (uint32_t i = 0; i < STEREO0D_NEIGHBORS; i += 1) {
    ret.data_.neighbor[i] = getIf<AT_NUM>(neighbors, i);
  }

  ret.data_.central_atom = getIf<AT_NUM>(stereo, "central_atom");

  ret.data_.type = getIf<S_CHAR>(stereo, "type");
  ret.data_.parity = getIf<S_CHAR>(stereo, "parity");

  return ret;
}

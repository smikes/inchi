/* molecule.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>
#include <uv.h>
#include <nan.h>

#include <cstring>

#include "./using_v8.h"

#include "./molecule.h"
#include "./get_inchi_worker.h"
#include "./inchi_lock.h"

/**

@module Internal
@class  Molecule_CC
 */

/* local (non-API) functions */
void populate_input(Handle<Value> val, Molecule* in);
static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result);
void addstring(Handle<Object> ret, const char * name, const char * value);

/**
 * Create an Molecule structure from a partially or fully specified
 * JavaScript object
 *
 * @method Create
 * @param {Handle&lt;Object&gt;} val Object handle that parallels an inchi_Input
 */
Molecule * Molecule::Create(Handle<Value> val) {
  // create an empty Molecule
  Molecule * input = new Molecule;

  // populate it
  populate_input(val, input);

  // return it
  return input;
}

void add_atom(Molecule* in, Handle<Object> a) {
  InchiAtom atom = InchiAtom::makeFromObject(a);

  int index = in->addAtom(atom);

  Handle<Array> neighbor =
    Handle<Array>::Cast(a->Get(NanSymbol("neighbor")));

  int bonds = neighbor->Length();
  for (int i = 0; i < bonds; ++i) {
    int bonded = neighbor->Get(i)->ToNumber()->Value();
    in->addBond(InchiBond(index, bonded));
  }
}

void populate_input(Handle<Value> val, Molecule* in) {
  // TODO(SOM): support validation, possibly return error code

  // expect args[0] to be an Object, call it 'mol'
  // expect mol.atom to be an Array
  // expect mol.options to be a string
  // expect mol.stereo0D to be an Array

  if (!val->IsObject()) {
    return;
  }
  Handle<Object> mol = val->ToObject();

  if (!mol->Has(NanSymbol("atom")) || !mol->Get(NanSymbol("atom"))->IsArray()) {
    return;
  }
  Handle<Array> atom = mol->Get(NanSymbol("atom")).As<Array>();

  int atoms = atom->Length();
  for (int i = 0; i < atoms; i += 1) {
    add_atom(in, atom->Get(i)->ToObject());
  }
}

void addstring(Handle<Object> ret, const char * name, const char * in) {
  const char * value = (in ? in : "");

  ret->Set(NanSymbol(name), String::New(value));
}

static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result) {
  addstring(ret, "inchi", out.szInChI);
  addstring(ret, "auxinfo", out.szAuxInfo);
  addstring(ret, "message", out.szMessage);
  addstring(ret, "log", out.szLog);
  ret->Set(NanSymbol("code"), Number::New(result));
}

Handle<Object> GetResult(GetINCHIData * data) {
  NanScope();

  Local<Object> ret = Object::New();

  populate_ret(ret, data->out_, data->result_);
  addstring(ret, "inchikey", data->inchikey);

  return scope.Close(ret);
}

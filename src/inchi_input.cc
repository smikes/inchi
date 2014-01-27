/* inchi_input.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>
#include <uv.h>
#include <nan.h>

#include "./inchi_input.h"

#include "./using_v8.h"

/**
 * Wrapper for INCHI_Input structure
 *
 * @module InChILib
 * @class  InchiInput
 */

/* local (non-API) functions */
static void populate_input(Handle<Value> val, inchi_Input* in);
static void populate_atom(const Handle<Object> atom, inchi_Atom* target);
static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result);
static void addstring(Handle<Object> ret,
                      const char * name, const char * value);



/**
 * Create an InchiInput structure from a partially or fully specified
 * JavaScript object
 *
 * @method Create
 * @param {Handle<Object>} val Object handle that parallels an inchi_Input
 */
InchiInput * InchiInput::Create(Handle<Value> val) {
  // validation

  // create an empty InchiInput
  InchiInput * input = new InchiInput;

  // populate it
  populate_input(val, &(input->in_));

  // return it
  return input;
}

int InchiInput::GetInchi() {
  result_ = GetINCHI(&(this->in_), &(this->out_));

  return result_;
}

Handle<Object> InchiInput::GetResult() {
  Local<Object> ret = Object::New();

  populate_ret(ret, this->out_, result_);

  return ret;
}

void populate_atom(const Handle<Object> atom, inchi_Atom* target) {
  // initialize atom
  memset(target, 0, sizeof(*target));

  // atom.name is required
  Handle<String> elname = atom->Get(NanSymbol("elname"))->ToString();

  NanCString(elname, 0, target->elname, ATOM_EL_LEN);
  target->elname[ATOM_EL_LEN-1] = '\0';  // ensure termination

  // populate neighbor array
  Handle<Array> neighbor =
    Handle<Array>::Cast(atom->Get(NanSymbol("neighbor")));

  target->num_bonds = neighbor->Length();
  for (int i = 0; i < target->num_bonds; ++i) {
    target->neighbor[i]    = neighbor->Get(i)->ToNumber()->Value();

    // TODO(SOM): bond types & stero too
    target->bond_type[i]   = INCHI_BOND_TYPE_SINGLE;
    target->bond_stereo[i] = INCHI_BOND_STEREO_NONE;
  }

  // default fill atoms with default isotopes
  target->num_iso_H[0] = -1;
}

static void populate_input(Handle<Value> val, inchi_Input* in) {
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
  Handle<Array> atom = Handle<Array>::Cast(mol->Get(NanSymbol("atom")));

  in->num_atoms = atom->Length();
  in->atom = new inchi_Atom[in->num_atoms];

  for (int i = 0; i < in->num_atoms; i += 1) {
    populate_atom(atom->Get(i)->ToObject(), &(in->atom[i]));
  }
}

static void addstring(Handle<Object> ret,
                      const char * name, const char * value) {
  if (value) {
    ret->Set(NanSymbol(name), String::New(value));
  } else {
    ret->Set(NanSymbol(name), String::New(""));
  }
}

static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result) {
  addstring(ret, "inchi", out.szInChI);
  addstring(ret, "auxinfo", out.szAuxInfo);
  addstring(ret, "message", out.szMessage);
  addstring(ret, "log", out.szLog);
  ret->Set(NanSymbol("code"), Number::New(result));
}

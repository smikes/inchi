/* node-inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>
#include <uv.h>

#include <algorithm>

#include "inchi_dll/inchi_api.h"
#include "inchi_dll/mode.h"

#include "./using_v8.h"


void register_GetINCHI_return_codes(Handle<Object> exports) {
  /**
   * Return codes for the GetINCHI/GetStdINCHI (old-style) functions
   * @class RetValGetInchi
   */
  /**
   * Success; no errors or warnings
   * @property {int} inchi_Ret_OKAY
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_OKAY);

  /**
   * Success; warning(s) issued
   * @property {int} inchi_Ret_WARNING
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_WARNING);

  /**
   * Error: no InCHI has been created
   * @property {int} inchi_Ret_ERROR
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_ERROR);

  /**
   * Severe error: no InCHI has been created
   *   (typically, memory allocation failure)
   * @property {int} inchi_Ret_FATAL
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_FATAL);

  /**
   * Unknown program error
   * @property {int} inchi_Ret_UNKNOWN
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_UNKNOWN);

  /**
   * Previous call to InChI has not returned yet
   * @property {int} inchi_Ret_BUSY
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_BUSY);

  /**
   * no structural data has been provided
   * @property {int} inchi_Ret_EOF
   * @final
   */
  NODE_DEFINE_CONSTANT(exports, inchi_Ret_EOF);
}

/**
 * Direct wrapper of the low-level C++ api
 *
 * http://www.inchi-trust.org/fileadmin/user_upload/software/inchi-v1.04/InChI_API_Reference.pdf
 *
 * @module InChILib
 * @class  InChILib
 */

/**
 * Returns the current version of the InChI algorithm
 * @method getAlgorithmVersion
 * @return {String} current algorithm version (from INCHI_VERSION)
 */
Handle<Value> getAlgorithmVersion(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New(INCHI_VERSION));
}

void populate_atom(const Handle<Object> atom, inchi_Atom* target) {
  // initialize atom
  memset(target, 0, sizeof(*target));

  // atom.name is required
  Handle<String> elname = atom->Get(String::NewSymbol("elname"))->ToString();
  elname->WriteAscii(target->elname, 0, ATOM_EL_LEN);
  target->elname[ATOM_EL_LEN-1] = '\0';  // ensure termination

  // populate neighbor array
  Handle<Array> neighbor =
    Handle<Array>::Cast(atom->Get(String::NewSymbol("neighbor")));

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

void populate_input(Handle<Value> val, inchi_Input& in) {
  // TODO(SOM): support validation, possibly return error code

  // expect args[0] to be an Object, call it 'mol'
  // expect mol.atom to be an Array
  // expect mol.options to be a string
  // expect mol.stereo0D to be an Array

  Handle<Object> mol = val->ToObject();

  Handle<Array> atom = Handle<Array>::Cast(mol->Get(String::NewSymbol("atom")));

  in.num_atoms = atom->Length();
  in.atom = new inchi_Atom[in.num_atoms];

  for (int i = 0; i < in.num_atoms; i += 1) {
    populate_atom(atom->Get(i)->ToObject(), &(in.atom[i]));
  }
}

void addstring(Handle<Object> ret, const char * name, const char * value) {
  if (value) {
    ret->Set(String::NewSymbol(name), String::New(value));
  } else {
    ret->Set(String::NewSymbol(name), String::New(""));
  }
}

/**
 * calls the "classic" GetINCHI function
 * @method GetINCHI
 * @return {inchi_Output}
 */
Handle<Value> GetINCHISync(const Arguments& args) {
  HandleScope scope;

  inchi_Input in = {0};
  inchi_Output out = {0};

  int result = inchi_Ret_OKAY;

  try {
    // TODO(SOM): populate inchi_Input
    populate_input(args[0], in);

    result = GetINCHI(&in, &out);
  } catch(...) {
    result = inchi_Ret_UNKNOWN;
  }

  Local<Object> ret = Object::New();

  addstring(ret, "inchi", out.szInChI);
  addstring(ret, "auxinfo", out.szAuxInfo);
  addstring(ret, "message", out.szMessage);
  addstring(ret, "log", out.szLog);
  ret->Set(String::NewSymbol("code"), Number::New(result));

  delete[] in.atom;

  FreeINCHI(&out);

  return scope.Close(ret);
};


void register_functions(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getAlgorithmVersion", getAlgorithmVersion);
  NODE_SET_METHOD(exports, "GetINCHISync", GetINCHISync);
}

void init(Handle<Object> exports) {
  register_GetINCHI_return_codes(exports);
  register_functions(exports);
}

NODE_MODULE(libinchi, init)

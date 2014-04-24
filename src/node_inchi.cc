/* node-inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <nan.h>

#include <algorithm>

#include "inchi_dll/inchi_api.h"
#include "inchi_dll/mode.h"

#include "./using_v8.h"

#include "./atom.h"
#include "./molecule.h"
#include "./molecule_wrap.h"
#include "./get_inchi.h"
#include "./get_struct_from_inchi.h"

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
NAN_METHOD(getAlgorithmVersion) {
  NanScope();

  NanReturnValue(String::New(INCHI_VERSION));
}


void register_functions(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getAlgorithmVersion", getAlgorithmVersion);
  NODE_SET_METHOD(exports, "GetINCHI", GetINCHI);
  NODE_SET_METHOD(exports, "GetInChIFromMolecule", GetInChIFromMolecule);
  NODE_SET_METHOD(exports, "GetStructFromINCHI", GetStructFromINCHI);
}

void init(Handle<Object> exports) {
  register_GetINCHI_return_codes(exports);
  register_functions(exports);
}


NODE_MODULE(inchi, init)

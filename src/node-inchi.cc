/* node-inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>
#include <uv.h>

#include <nan.h>

#include <algorithm>

#include "inchi_dll/inchi_api.h"
#include "inchi_dll/mode.h"

#include "./using_v8.h"

#include "./inchi_input.h"

NAN_METHOD(GetINCHI);

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
NAN_METHOD(getAlgorithmVersion) {
  NanScope();

  NanReturnValue(String::New(INCHI_VERSION));
}


/**
 * calls the "classic" GetINCHI function
 * @method GetINCHI
 * @return {inchi_Output}
 */
NAN_METHOD(GetINCHISync) {
  NanScope();

  InchiInput * input = NULL;
  InchiInput::GetINCHIData * data = NULL;
  Handle<Object> ret;

  try {
    // TODO(SOM): validate args
    Handle<Value> mol = args[0];

    input = InchiInput::Create(mol);

    data = input->tearOffGetINCHIData();

    data->GetInchi();

    ret = data->GetResult();
  } catch(...) {
    ret = Object::New();

    ret->Set(NanSymbol("code"), Number::New(inchi_Ret_UNKNOWN));
  }

  delete data;
  delete input;

  NanReturnValue(ret);
};


void register_functions(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getAlgorithmVersion", getAlgorithmVersion);
  NODE_SET_METHOD(exports, "GetINCHISync", GetINCHISync);
  NODE_SET_METHOD(exports, "GetINCHI", GetINCHI);
}

void init(Handle<Object> exports) {
  register_GetINCHI_return_codes(exports);
  register_functions(exports);
}

NODE_MODULE(inchi, init)

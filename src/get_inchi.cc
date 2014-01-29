/* get_inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./using_v8.h"

#include "./get_inchi.h"

#include "./inchi_input.h"
#include "./get_inchi_worker.h"

#include "inchi_dll/inchi_api.h"

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
 Constructs a molecule and calls the GetINCHI function.

 This is a *synchronous* version of the GetINCHI API and should
 not generally be used.

 The return value is an Object containing the the result code of
 calling GetINCHI.  If the computation was successful, there will
 be additional members with the InChI string, InChIKey, etc.
 See {{#crossLink "GetINCHIResult"}}GetINCHIResult{{/crossLink}} for
 full documentation

 @method GetINCHISync
 @for InChILib
 @param {Object} molecule Object defining the molecule, in the format:

     var methanol = {
         atom: [
            { 'elname': 'C', neighbor: [1] },
            { 'elname': 'O' }
         ]
     };

 @return {GetINCHIResult} output Object containing result of GetINCHI
 */
NAN_METHOD(GetINCHISync) {
  NanScope();

  InchiInput * input = NULL;
  GetINCHIData * data = NULL;
  Handle<Object> ret;

  try {
    // TODO(SOM): validate args
    Handle<Value> mol = args[0];

    input = InchiInput::Create(mol);

    data = input->tearOffGetINCHIData();

    data->GetInchi();

    ret = GetResult(data);
  } catch(...) {
    ret = Object::New();

    ret->Set(NanSymbol("code"), Number::New(inchi_Ret_UNKNOWN));
  }

  delete data;
  delete input;

  NanReturnValue(ret);
};

/**
 Constructs a molecule and calls the GetINCHI function.

 This is an asynchronous version of the GetINCHI API suitable
 for general use.

 The second argument passed to callback value is an Object containing the the result code of
 calling GetINCHI.  If the computation was successful, there will
 be additional members with the InChI string, InChIKey, etc.
 See {{#crossLink "GetINCHIResult"}}GetINCHIResult{{/crossLink}} for
 full documentation

 @method GetINCHI
 @for InChILib
 @param {Object} molecule Object defining the molecule, in the format

     var methanol = {
         atom: [
            { 'elname': 'C', neighbor: [1] },
            { 'elname': 'O' }
         ]
     };

 @param {Function} callback Callback Function
 @param {String} callback.err Error encountered during the operation
 @param {GetINCHIResult} callback.output Object containing result of GetINCHI
 */
NAN_METHOD(GetINCHI) {
  NanScope();

  InchiInput * input = NULL;
  Handle<Object> ret;

  try {
    // TODO(SOM): validate args
    Handle<Value> mol = args[0];
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    input = InchiInput::Create(mol);

    NanAsyncQueueWorker(new GetINCHIWorker(callback, input));
  } catch(...) {
  }

  delete input;

  NanReturnUndefined();
};

/**
 The GetINCHIResult Object will always contain a ``code`` member which
 is the return code from the GetINCHI function.

 When the ``code`` represents success (or success-with-warning), the
 members ``inchi`` and ``inchikey`` will also be present.

 When the ``code`` represents an error, the ``message`` member may be
 present, giving more information about the error.

 @module InChILib
 @class GetINCHIResult
 */
/**
 * Result code from call to GetINCHI
 * @property {RetValGetInchi} code
 */
/**
 * InChI identifier for the specified input
 * @property {String} inchi
 */
/**
 * InChIKey hash for the returned InChI
 * @property {String} inchikey
 */
/**
 * Auxiliary info calculated during the computation
 * @property {String} auxinfo
 */
/**
 * Error or warning messages, if any, generated during the computation
 * @property {String} message
 */
/**
 * Log messages generated during the computation
 * @property {String} log
 */

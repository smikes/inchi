/* inchi_input.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <v8.h>
#include <uv.h>
#include <nan.h>

#include <cstring>

#include "./inchi_input.h"

#include "./using_v8.h"

/**

@module Internal
@class  InchiInput
 */

/* local (non-API) functions */
static void populate_input(Handle<Value> val, InchiInput* in);
static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result);
static void addstring(Handle<Object> ret,
                      const char * name, const char * value);



/**
 * Create an InchiInput structure from a partially or fully specified
 * JavaScript object
 *
 * @method Create
 * @param {Handle&lt;Object&gt;} val Object handle that parallels an inchi_Input
 */
InchiInput * InchiInput::Create(Handle<Value> val) {
  // TODO(SOM): validation

  // create an empty InchiInput
  InchiInput * input = new InchiInput;

  // populate it
  populate_input(val, input);

  // return it
  return input;
}


struct GetINCHIWorker : public NanAsyncWorker {
  GetINCHIData * data_;

  GetINCHIWorker(NanCallback * callback, InchiInput* input) :
    NanAsyncWorker(callback) {
    data_ = input->tearOffGetINCHIData();
  }
  ~GetINCHIWorker() {
    delete data_;
  }

  void Execute() {
    data_->GetInchi();
  }

  void HandleOKCallback() {
    NanScope();

    Handle<Object> result = GetResult(data_);

    Local<Value> argv[] = {
      NanNewLocal<Value>(v8::Null()),
      NanNewLocal<Value>(result)
    };

    callback->Call(2, argv);
  }
};

/**
 * @class InChILib
 */

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



void add_atom(InchiInput* in, Handle<Object> atom) {
  Handle<String> elname_string = atom->Get(NanSymbol("elname"))->ToString();

  char * elname = NanCString(elname_string, 0);
  int index = in->addAtom(elname);

  Handle<Array> neighbor =
    Handle<Array>::Cast(atom->Get(NanSymbol("neighbor")));

  int bonds = neighbor->Length();
  for (int i = 0; i < bonds; ++i) {
    int bonded = neighbor->Get(i)->ToNumber()->Value();
    in->addBond(InchiBond(index, bonded));
  }
}

static void populate_input(Handle<Value> val, InchiInput* in) {
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

  int atoms = atom->Length();
  for (int i = 0; i < atoms; i += 1) {
    add_atom(in, atom->Get(i)->ToObject());
  }
}

static void addstring(Handle<Object> ret,
                      const char * name, const char * in) {
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
  Local<Object> ret = Object::New();

  populate_ret(ret, data->out_, data->result_);
  addstring(ret, "inchikey", data->inchikey);

  return ret;
}

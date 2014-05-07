/* molecule.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <nan.h>
#include <node.h>
#include <v8.h>

#include "./using_v8.h"

#include "./molecule.h"
#include "./get_inchi_worker.h"

#include "./node_inchi.h"

/**

@module Internal
@class  Molecule_CC
 */

/* local (non-API) functions */
static void populate_ret(Handle<Object> ret,
                         const inchi_Output& out, int result);
void addstring(Handle<Object> ret, const char * name, const char * value);

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
  ret->Set(NanSymbol("result"), Number::New(result));
}

Handle<Object> GetResult(GetINCHIData * data) {
  NanScope();

  Local<Object> ret = Object::New();

  populate_ret(ret, data->out_, data->result_);
  addstring(ret, "inchikey", data->inchikey);

  return scope.Close(ret);
}

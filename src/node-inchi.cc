#include <node.h>
#include <v8.h>
#include <uv.h>

#include "../INCHI-1-API/INCHI_API/inchi_dll/inchi_api.h"
#include "../INCHI-1-API/INCHI_API/inchi_dll/mode.h"

using namespace v8;

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

Handle<Value> StructureToInChI(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));

  // build INCHI_Input structure from args

  //

}

Handle<Value> getAlgorithmVersion(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New(INCHI_VERSION));
}


void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("hello"),
               FunctionTemplate::New(Method)->GetFunction());
  exports->Set(String::NewSymbol("getAlgorithmVersion"),
               FunctionTemplate::New(getAlgorithmVersion)->GetFunction());
}

NODE_MODULE(libinchi, init)

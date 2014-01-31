/* atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "./atom.h"

NAN_METHOD(Atom::New) {
  NanScope();

  // check args - one optional Name arg
  Atom * atom = NULL;

  if (args.Length() == 0 || (!args[0]->IsString())) {
    atom = new Atom();
  } else {
    char *name = NanCString(args[0], 0);
    atom = new Atom(name);
    delete [] name;
  }

  atom->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(Atom::GetName) {
  NanScope();

  Atom * atomWrap = ObjectWrap::Unwrap<Atom>(args.This());

  NanReturnValue(v8::String::New(atomWrap->atom.getName().c_str()));
}

Atom::Atom(const char *name) : atom(name) {}


static v8::Persistent<v8::Function> atom_constructor;

void Atom::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(Atom::New);
  tpl->SetClassName(NanSymbol("Atom"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "getName", Atom::GetName);

  atom_constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

  exports->Set(NanSymbol("Atom"), atom_constructor);
}

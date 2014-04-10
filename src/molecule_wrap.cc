/* molecule_wrap.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "./molecule_wrap.h"

#include "./using_v8.h"

#include "./get_inchi.h"
#include "./get_inchi_data.h"
#include "./get_inchi_worker.h"

static Persistent<Function> m_molecule_wrap_constructor;


void Molecule_wrap::Init(v8::Handle<v8::Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(Molecule_wrap::New);
  tpl->SetClassName(NanSymbol("Molecule"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "GetInChI", Molecule_wrap::GetInChI);

  m_molecule_wrap_constructor = Persistent<Function>::New(tpl->GetFunction());

  exports->Set(NanSymbol("Molecule"), m_molecule_wrap_constructor);
}

NAN_METHOD(Molecule_wrap::New) {
  NanScope();

  // check for arguments
  Molecule_wrap * m = new Molecule_wrap;

  Handle<Object> in = args[0]->ToObject();

  m->addAtoms(in->Get(NanSymbol("atoms")));

  m->addBonds(in->Get(NanSymbol("bonds")));

  m->addStereo(in->Get(NanSymbol("stereo0D")));
  m->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(Molecule_wrap::GetInChI) {
  NanScope();

  Molecule_wrap * m = ObjectWrap::Unwrap<Molecule_wrap>(args.This());
  NanCallback * callback = new NanCallback(args[0].As<Function>());

  NanAsyncQueueWorker(new GetINCHIWorker(callback, &(m->mol)));

  NanReturnUndefined();
}


void Molecule_wrap::addAtoms(Handle<Value> a) {
  Handle<Array> atoms = a.As<Array>();

  // atoms is an array of v8::String
  for (uint32_t i = 0; i < atoms->Length(); i += 1) {
    char * elname = NanCString(atoms->Get(i), 0);
    mol.addAtom(elname);
    delete[] elname;
  }
}

void Molecule_wrap::addBonds(Handle<Value> b) {
  Handle<Array> bonds = b.As<Array>();

  // bonds is an array of v8::Object
  for (uint32_t i = 0; i < bonds->Length(); i += 1) {
    Handle<Object> bond = bonds->Get(i)->ToObject();

    int from  = bond->Get(NanSymbol("from"))->NumberValue();
    int to    = bond->Get(NanSymbol("to"))->NumberValue();
    int order = bond->Get(NanSymbol("order"))->NumberValue();

    InchiBond b = (order ? InchiBond(from, to, order) : InchiBond(from, to));
    mol.addBond(b);
  }
}

void Molecule_wrap::addStereo(Handle<Value> s) {
  if (s->IsNull() || s->IsUndefined()) {
    return;
  }

  Handle<Array> stereos = s.As<Array>();

  for (uint32_t i = 0; i < stereos->Length(); i += 1) {
    Handle<Object> stereo = stereos->Get(i)->ToObject();

    mol.addStereo(InchiStereo::makeFromObject(stereo));
  }
}

const InchiStereo InchiStereo::makeFromObject(Handle<Object> stereo) {
  InchiStereo ret;

  Local<Array> neighbors = stereo->Get(NanSymbol("neighbor")).As<Array>();
  for (uint32_t i = 0; i < STEREO0D_NEIGHBORS; i += 1) {
    ret.data_.neighbor[i] = AT_NUM(neighbors->Get(i)->NumberValue());
  }

  ret.data_.central_atom = AT_NUM(stereo->Get(NanSymbol("central_atom"))->NumberValue());

  ret.data_.type = S_CHAR(stereo->Get(NanSymbol("type"))->NumberValue());
  ret.data_.parity = S_CHAR(stereo->Get(NanSymbol("parity"))->NumberValue());

  return ret;
}

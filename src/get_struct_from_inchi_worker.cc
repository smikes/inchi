/* get_struct_from_inchi_worker.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include <string>

#include "./using_v8.h"

#include "./get_struct_from_inchi_data.h"
#include "./get_struct_from_inchi_worker.h"

#include "./inchi_queue.h"
#include "./inchi_lock.h"

Handle<Object> MakeStructure(const GetStructFromINCHIData& data);

void GetStructFromINCHIWorker::HandleOKCallback() {
  NanScope();

  Handle<Object> result = MakeStructure(data_);

  Handle<Value> argv[] = {
    v8::Null(),
    result
  };

  callback->Call(2, argv);
}

void GetStructFromINCHIWorker::Execute() {
    Inchi_Global_Lock __lock;
    data_.GetStructFromINCHI();
}

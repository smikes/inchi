/* get_inchi_worker.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./using_v8.h"

#include "./get_inchi_worker.h"

Handle<Object> GetResult(GetINCHIData * data);

void GetINCHIWorker::HandleOKCallback() {
  NanScope();

  QueueFinish();

  Handle<Object> result = GetResult(data_);

  Handle<Value> argv[] = {
    NanNewLocal<Value>(v8::Null()),
    NanNewLocal<Value>(result)
  };

  callback->Call(2, argv);
}

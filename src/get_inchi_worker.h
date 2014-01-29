#ifndef SRC_GET_INCHI_WORKER_H
#define SRC_GET_INCHI_WORKER_H
/* get_inchi_worker.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

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

#endif  // SRC_GET_INCHI_WORKER_H

#ifndef SRC_GET_INCHI_WORKER_H_
#define SRC_GET_INCHI_WORKER_H_
/* get_inchi_worker.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include "./molecule.h"
#include "./get_inchi_data.h"

#include "./inchi_queue.h"

/**
   This class, derived from NanAsyncWorker, manages asynchronous
   calls to GetINCHI

   @module Internal
   @class GetINCHIWorker
*/
struct GetINCHIWorker : public NanAsyncWorker {
  GetINCHIData * data_;

/**
   Tears off a copy of the current molecule representation,
   so the Molecule is still safe to access on the v8 thread
   while GetINCHI runs on another thread

   @method GetINCHIWorker
   @constructor
   @param {NanCallback *} callback Callback function to call when finished
   @param {Molecule *} input     Molecule representation
 */
  GetINCHIWorker(NanCallback * callback, Molecule* input) :
    NanAsyncWorker(callback) {
    data_ = input->tearOffGetINCHIData();
  }

/**
 @method ~GetINCHIWorker
*/
  ~GetINCHIWorker() {
    delete data_;
  }

/**
   Calls GetINCHI with thread-safe data

   @method Execute
*/
  void Execute();

/**
   Constructs result value, passes it to callback

   @method HandleOKCallback
*/
  void HandleOKCallback();
};

#endif  // SRC_GET_INCHI_WORKER_H_

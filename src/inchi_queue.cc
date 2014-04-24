/* inchi_queue.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <nan.h>

#include <deque>

#include "./using_v8.h"
#include "./inchi_lock.h"

static bool running;
static std::deque<NanAsyncWorker *> queue;

void Enqueue(NanAsyncWorker* w) {
  Inchi_Global_Lock __lock;

  if (!running) {
    NanAsyncQueueWorker(w);
    running = true;
  } else {
    queue.push_back(w);
  }
}

void QueueFinish() {
  Inchi_Global_Lock __lock;

  running = false;

  if (!queue.empty()) {
    NanAsyncQueueWorker(queue.front());

    queue.pop_front();

    running = true;
  }
}

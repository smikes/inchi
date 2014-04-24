#ifndef SRC_INCHI_QUEUE_H_
#define SRC_INCHI_QUEUE_H_
/* inchi_queue.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <nan.h>

void Enqueue(NanAsyncWorker* w);
void QueueFinish();

#endif  // SRC_INCHI_QUEUE_H_

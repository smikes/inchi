/* inchi_lock.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <node.h>
#include <uv.h>

#include "./inchi_lock.h"

static uv_mutex_t mutex;
struct Inchi_Lock_Initializer {
  Inchi_Lock_Initializer() {
    uv_mutex_init(&mutex);
  }
} __lock_init;


Inchi_Global_Lock::Inchi_Global_Lock() {
  uv_mutex_lock(&mutex);
}

Inchi_Global_Lock::~Inchi_Global_Lock() {
  uv_mutex_unlock(&mutex);
}

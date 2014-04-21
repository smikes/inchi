#ifndef SRC_INCHI_LOCK_H_
#define SRC_INCHI_LOCK_H_
/* inchi_lock.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

struct Inchi_Global_Lock {
  Inchi_Global_Lock();
  ~Inchi_Global_Lock();
};

#endif  // SRC_INCHI_LOCK_H_

#ifndef SRC_TEST_TESTHELPERS_H_
#define SRC_TEST_TESTHELPERS_H_
/* TestHelpers
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <string>

#include "inchi_dll/inchi_api.h"

inline SimpleString StringFrom (const std::string & s) {
  return StringFrom(s.c_str());
}

inline SimpleString StringFrom (const S_CHAR n) {
  return StringFrom(long(n));
}

inline SimpleString StringFrom (const AT_NUM n) {
  return StringFrom(long(n));
}

#endif  // SRC_TEST_TESTHELPERS_H_

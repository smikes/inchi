/* test_get_struct_from_inchi.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/* test for the native side of the inchi->structure code */
#include <string>

#include "CppUnitLite/TestHarness.h"
#include "./TestHelpers.h"

#include "get_struct_from_inchi_data.h"

TEST(GetStructFromINCHIData, creation) {
  GetStructFromINCHIData data("");

  CHECK_EQUAL(data.in_.szInChI, (const char *)NULL);
  CHECK_EQUAL(data.in_.szOptions, (const char *)NULL);
}

TEST(GetStructFromINCHIData, setInchi) {
  GetStructFromINCHIData data("");

  data.setInchi("InChI=1S/CH4/h1H4");

  CHECK_EQUAL(std::string("InChI=1S/CH4/h1H4"), data.inchi_);
}

TEST(GetStructFromINCHIData, getStruct_methane) {
  GetStructFromINCHIData data("InChI=1S/CH4/h1H4");

  int result = data.GetStructFromINCHI();

  CHECK_EQUAL(long(inchi_Ret_OKAY), long(result));

  CHECK_EQUAL(AT_NUM(1), data.out_.num_atoms);
  CHECK_EQUAL(std::string("C"), data.out_.atom[0].elname);
}

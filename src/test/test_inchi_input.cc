/* test_inchi_input.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/* test for the native side of the InchiInput code */
#include "CppUnitLite/TestHarness.h"
#include "inchi_input.h"

TEST(InchiInput, creation)
{
  InchiInput in;
}

TEST(InchiInput, addAtom)
{
  InchiInput in;

  in.addAtom("O");
}

TEST(InchiInput, addAnotherAtom)
{
  InchiInput in;

  in.addAtom("O");
  int result = in.addAtom("C");

  LONGS_EQUAL(result, 1);
}

TEST(InchiInput, atomsHaveNames)
{
  InchiInput in;

  in.addAtom("C");

  SimpleString c("C");

  CHECK_EQUAL(in.getAtom(0).elname, c);
}

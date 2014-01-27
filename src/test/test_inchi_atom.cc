/* test_inchi_atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/* test for the native side of the InchiInput code */
#include "CppUnitLite/TestHarness.h"
#include "inchi_atom.h"
#include "inchi_atom.h"

SimpleString StringFrom (const std::string & s) {
  return StringFrom(s.c_str());
}

TEST(InchiAtom, creation)
{
  InchiAtom a;
}

TEST(InchiAtom, atomsHaveNames)
{
  InchiAtom a("C");

  CHECK_EQUAL(a.getName(), "C");
}

TEST(InchiAtom, canSetName)
{
  InchiAtom a;

  a.setName("Tc");

  CHECK_EQUAL(a.getName(), "Tc");
}

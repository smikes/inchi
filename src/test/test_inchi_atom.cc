/* test_inchi_atom.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/* test for the native side of the InchiInput code */
#include "CppUnitLite/TestHarness.h"
#include "./TestHelpers.h"

#include "./inchi_atom.h"
#include "./inchi_atom.h"


TEST(InchiAtom, creation)
{
  InchiAtom a;
}

TEST(InchiAtom, atomsHaveNames)
{
  InchiAtom a("C");

  CHECK_EQUAL("C", a.getName());
}

TEST(InchiAtom, canSetName)
{
  InchiAtom a;

  a.setName("Tc");

  CHECK_EQUAL("Tc", a.getName());
}

TEST(InchiAtom, shouldTruncateTooLongName)
{
  InchiAtom a;

  a.setName("AVeryLongName");

  CHECK_EQUAL("AVery", a.getName());
}

TEST(InchiAtom, hasDefaultIsotopes)
{
  InchiAtom c("C");

  CHECK_EQUAL(InchiAtom::IMPLICIT_NONISOTOPIC_H,
              c.num_iso_H[InchiAtom::NON_ISOTOPIC_H]);
}

TEST(InchiAtom, conversion)
{
  InchiAtom c("C");

  inchi_Atom a = c;

  CHECK_EQUAL(std::string("C"), a.elname);
}

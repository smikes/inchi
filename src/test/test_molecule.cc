/* test_inchi_input.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

/* test for the native side of the InchiInput code */
#include <string>

#include "CppUnitLite/TestHarness.h"
#include "./TestHelpers.h"

#include "molecule.h"
#include "molecule.h"

TEST(Molecule, creation)
{
  Molecule in;
}

TEST(Molecule, addAtom)
{
  Molecule in;

  in.addAtom("O");
}

TEST(Molecule, addAnotherAtom)
{
  Molecule in;

  in.addAtom("O");
  int result = in.addAtom("C");

  LONGS_EQUAL(result, 1);
}

TEST(Molecule, atomsHaveNames)
{
  Molecule in;

  in.addAtom("C");

  SimpleString c("C");

  CHECK_EQUAL(in.getAtom(0).elname, c);
}

TEST(Molecule, makeFormaldehyde)
{
  Molecule in;

  in.addAtom("C");
  in.addAtom("O");

  in.addBond(InchiBond(0, 1, INCHI_BOND_TYPE_DOUBLE));

  GetINCHIData * data = in.tearOffGetINCHIData();

  data->GetInchi();

  CHECK_EQUAL(std::string("InChI=1S/CH2O/c1-2/h1H2"), data->out_.szInChI);
  CHECK_EQUAL(std::string("WSFSSNUMVMOOMR-UHFFFAOYSA-N"), data->inchikey);

  delete data;
}
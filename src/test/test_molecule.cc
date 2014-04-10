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

  std::string c("C");

  CHECK_EQUAL(in.getAtom(0).getName(), c);
}

TEST(Molecule, makeMethanol)
{
  Molecule in;

  in.addAtom("C");
  in.addAtom("O");

  in.addBond(InchiBond(0, 1));

  GetINCHIData * data = in.tearOffGetINCHIData();

  data->GetInchi();

  CHECK_EQUAL(std::string("InChI=1S/CH4O/c1-2/h2H,1H3"), data->out_.szInChI);
  CHECK_EQUAL(std::string("OKKJLVBELUTLKV-UHFFFAOYSA-N"), data->inchikey);

  delete data;
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

TEST(Molecule, fromInchiMethanol)
{
  Molecule * methanol = Molecule::fromInchi("InChI=1S/CH4O/c1-2/h2H,1H3");

  CHECK_EQUAL(AT_NUM(2), methanol->getAtomCount());

  CHECK_EQUAL(std::string("C"), methanol->getAtom(0).getName());
  CHECK_EQUAL(std::string("O"), methanol->getAtom(1).getName());

  delete methanol;
}

TEST(Molecule, fructoseRoundTrip)
{
  std::string fructoseInchi =
    "InChI=1S/C6H12O6/c7-1-3(9)5(11)6(12)4(10)2-8/h3,5-9,11-12H,1-2H2/t3-,5-,6-/m1/s1";
  Molecule * fructose = Molecule::fromInchi(fructoseInchi);

  CHECK_EQUAL(AT_NUM(15), fructose->getAtomCount());

  GetINCHIData * data = fructose->tearOffGetINCHIData();

  data->GetInchi();

  CHECK_EQUAL(fructoseInchi, data->out_.szInChI);

  delete data;
  delete fructose;
}

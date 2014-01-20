#include "CppUnitLite/TestHarness.h"
#include "inchi_input.h"

TEST( InchiInput, creation )
{
  InchiInput in;

  LONGS_EQUAL(in.num_atoms, 0);
}

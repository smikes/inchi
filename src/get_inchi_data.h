#ifndef SRC_GET_INCHI_DATA_H_
#define SRC_GET_INCHI_DATA_H_
/* get_inchi_data.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include "inchi_dll/inchi_api.h"

struct GetINCHIData {
  static const int INCHIKEY_SIZE = 28;

  GetINCHIData();
  ~GetINCHIData();

  inchi_Input in_;
  inchi_Output out_;
  char inchikey[INCHIKEY_SIZE];
  int result_;

  int GetInchi();
};

#endif  // SRC_GET_INCHI_DATA_H_

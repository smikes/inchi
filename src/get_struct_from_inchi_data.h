#ifndef SRC_GET_STRUCT_FROM_INCHI_DATA_H_
#define SRC_GET_STRUCT_FROM_INCHI_DATA_H_
/* get_inchi_data.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <string>

#include "inchi_dll/inchi_api.h"

struct GetStructFromINCHIData {
  explicit GetStructFromINCHIData(const std::string& inchi);
  ~GetStructFromINCHIData();

  std::string inchi_;
  std::string options_;

  inchi_InputINCHI in_;
  inchi_OutputStruct out_;
  int result_;

  void setInchi(const std::string&);

  int GetStructFromINCHI();
};

#endif  // SRC_GET_STRUCT_FROM_INCHI_DATA_H_

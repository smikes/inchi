/* get_inchi_data.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <cstring>

#include "./get_inchi_data.h"

/**
 *
 * @module Internal
 * @class GetINCHIData
 */

GetINCHIData::GetINCHIData() {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
  strncpy(inchikey, "", INCHIKEY_SIZE);
  result_ = inchi_Ret_UNKNOWN;
}

GetINCHIData::~GetINCHIData() {
  FreeINCHI(&out_);
  delete in_.atom;
  delete in_.stereo0D;
}

/**
 * calculate INCHI from structure
 *
 * @method GetInchi
 * @return {RetValGetInchi} result code from GetINCHI API call
 */
int GetINCHIData::GetInchi() {
  this->result_ = GetINCHI(&(this->in_), &(this->out_));

  if (this->result_ == inchi_Ret_OKAY) {
    GetINCHIKeyFromINCHI(this->out_.szInChI, 0, 0,
                         this->inchikey, 0, 0);
  }

  return this->result_;
}

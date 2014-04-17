/* get_inchi_data.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <cstring>

#include "./get_inchi_data.h"
#include "./inchi_lock.h"

/**
   "Tear-off" class that contains inchi_Input and inchi_Output
   structures for calling GetINCHI.

   Also stores result value and calculates InChIKey on success.

   @module Internal
   @class GetINCHIData
 */
/**

   @property in_
   @type inchi_Input
*/
/**

   @property out_
   @type inchi_Output
*/
/**
   The result code from calling GetINCHI

   @property result_
   @type RetValGetIchi
*/
/**
   InChIKey calculated from returned InChI only when
   return code indicates success

   @property inchikey_
   @type inchi_Input
*/


/**
 Constructor, creates empty data

 @constructor
 @method GetINCHIData
*/
GetINCHIData::GetINCHIData() {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
  strncpy(inchikey, "", INCHIKEY_SIZE);
  result_ = inchi_Ret_UNKNOWN;
}

/**
 Destructor; calls FreeINCHI and delete

 @method ~GetINCHIData
*/
GetINCHIData::~GetINCHIData() {
  FreeINCHI(&out_);
  delete[] in_.atom;
  delete[] in_.stereo0D;
}

/**
   calculate INCHI from structure

   @method GetInchi
   @for GetINCHIData
   @return {RetValGetInchi} result code from GetINCHI API call
 */
int GetINCHIData::GetInchi() {
  Inchi_Global_Lock __lock;

  this->result_ = GetINCHI(&(this->in_), &(this->out_));

  if (this->result_ == inchi_Ret_OKAY) {
    GetINCHIKeyFromINCHI(this->out_.szInChI, 0, 0,
                         this->inchikey, 0, 0);
  }

  return this->result_;
}

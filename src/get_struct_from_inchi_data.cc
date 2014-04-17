/* get_struct_from_inchi_data.cc
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */
#include <cstring>

#include "./get_struct_from_inchi_data.h"
#include "./inchi_lock.h"

/**
  "Tear-off" class that contains inchi_InputINCHI and inchi_OutputStruct
  structures for calling GetStructFromINCHI.

  Also stores result value.

  @module Internal
  @class GetStructFromINCHIData
*/
/**
  @property in_
  @type inchi_InputINCHI
*/
/**

   @property out_
   @type inchi_OutputStruct
*/

/**
  Constructor, creates empty data

  @constructor
  @method GetStructFromINCHIData
*/
GetStructFromINCHIData::GetStructFromINCHIData(const std::string& inchi)
  : inchi_(inchi) {
  memset(&in_, 0, sizeof(in_));
  memset(&out_, 0, sizeof(out_));
  result_ = inchi_Ret_UNKNOWN;
}

/**
  Destructor; calls FreeStructFromINCHI and delete

  @method ~GetStructFromINCHIData
*/
GetStructFromINCHIData::~GetStructFromINCHIData() {
  FreeStructFromINCHI(&out_);
}

/**
  setInchi: set inchi string used

  @method setInchi
  @param {const std::string&} inchi InChI to build structure from
*/
void GetStructFromINCHIData::setInchi(const std::string& s) {
  inchi_ = s;
}


/**
   calculate INCHI from structure

   @method GetStructFromINCHI
   @return {RetValGetInchi} result code from GetStructFromINCHI API call
 */
int GetStructFromINCHIData::GetStructFromINCHI() {
  this->in_.szInChI = const_cast<char *>(inchi_.c_str());
  this->in_.szOptions = const_cast<char *>(options_.c_str());

  {
    Inchi_Global_Lock __lock;
    this->result_ = ::GetStructFromINCHI(&(this->in_), &(this->out_));
  }

  return this->result_;
}

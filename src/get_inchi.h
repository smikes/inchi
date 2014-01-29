#ifndef SRC_GET_INCHI_H
#define SRC_GET_INCHI_H
/* get_inchi.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

#include <node.h>
#include <v8.h>

#include <nan.h>

#include "./using_v8.h"

NAN_METHOD(GetINCHI);
NAN_METHOD(GetINCHISync);

void register_GetINCHI_return_codes(Handle<Object> exports);

#endif  // SRC_GET_INCHI_H

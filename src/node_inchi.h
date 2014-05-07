#ifndef SRC_NODE_INCHI_H_
#define SRC_NODE_INCHI_H_
/* node_inchi.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

void register_GetINCHI_return_codes(Handle<Object> exports);

const InchiAtom InchiAtom_makeFromObject(Handle<Object> atom);
const InchiBond InchiBond_makeFromObject(Handle<Object> bond);
const InchiStereo InchiStereo_makeFromObject(Handle<Object> stereo);


#endif  // SRC_NODE_INCHI_H_

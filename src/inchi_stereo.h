#ifndef SRC_INCHI_STEREO_H_
#define SRC_INCHI_STEREO_H_
/* inchi_stereo.h
 * Copyright 2014 Cubane Canada, Inc.
 *
 * Released under the MIT license -- see MIT-LICENSE for details
 */

const int STEREO0D_NEIGHBORS=4;

/**
    Internal class for representing 1-atom stereochemistry, irrespective of dimenstion

    @class InchiStereo
    @module Internal
*/

struct InchiStereo {
  inchi_Stereo0D data_;

  /**
     Constructs an instance of a 0d stereo datum

     (can just use reference to inchi_Stereo0D struct as that is POD only)

     @constructor
     @method InchiStereo
   */
  InchiStereo() {
    memset(&data_, 0, sizeof(data_));
  }
  ~InchiStereo() {
  }

  explicit InchiStereo(const inchi_Stereo0D& s) : data_(s) {
  }

  static const InchiStereo makeFromObject(Handle<Object>);

  operator const inchi_Stereo0D() {
    return data_;
  }
};

#endif  // SRC_INCHI_STEREO_H_

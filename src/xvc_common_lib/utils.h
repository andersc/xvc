/******************************************************************************
* Copyright (C) 2017, Divideon. All rights reserved.
* No part of this code may be reproduced in any form
* without the written permission of the copyright holder.
******************************************************************************/

#ifndef XVC_COMMON_LIB_UTILS_H_
#define XVC_COMMON_LIB_UTILS_H_

#include <cassert>

#include "xvc_common_lib/common.h"

namespace xvc {

namespace util {

constexpr bool IsLuma(YuvComponent comp) {
  return comp == YuvComponent::kY;
}

constexpr bool IsFirstChroma(YuvComponent comp) {
  return comp == YuvComponent::kU;
}

template <typename T, typename U>
static T Clip3(U value, T min, T max) {
  if (value < min) return min;
  if (value > max) return max;
  return static_cast<T>(value);
}

template <typename T>
static Sample ClipBD(T value, Sample max) {
  if (value < 0) return 0;
  if (value > max) return max;
  return static_cast<Sample>(value);
}

int SizeToLog2(int size);
int SizeLog2Bits(int size);

int GetChromaShiftX(ChromaFormat chroma_format);
int GetChromaShiftY(ChromaFormat chroma_format);

int ScaleChromaX(int size, ChromaFormat chroma_format);
int ScaleChromaY(int size, ChromaFormat chroma_format);

inline int ScaleSizeX(int size, ChromaFormat chroma_format,
                      YuvComponent comp) {
  return IsLuma(comp) ? size : ScaleChromaX(size, chroma_format);
}

inline int ScaleSizeY(int size, ChromaFormat chroma_format,
                      YuvComponent comp) {
  return IsLuma(comp) ? size : ScaleChromaY(size, chroma_format);
}

inline int GetLumaNumSamples(int width, int height) {
  return width * height;
}

inline int GetChromaNumSamples(int width, int height, ChromaFormat chroma_fmt) {
  return ScaleChromaX(width, chroma_fmt) * ScaleChromaY(height, chroma_fmt);
}

inline int GetTotalNumSamples(int width, int height, ChromaFormat chroma_fmt) {
  return GetLumaNumSamples(width, height) +
    (GetChromaNumSamples(width, height, chroma_fmt) << 1);
}

inline int GetNumComponents(ChromaFormat chroma_fmt) {
  return chroma_fmt == ChromaFormat::kMonochrome ? 1 : 3;
}

}   // namespace util

}   // namespace xvc

#endif  // XVC_COMMON_LIB_UTILS_H_
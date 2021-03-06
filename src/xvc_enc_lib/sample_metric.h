/******************************************************************************
* Copyright (C) 2017, Divideon.
*
* Redistribution and use in source and binary form, with or without
* modifications is permitted only under the terms and conditions set forward
* in the xvc License Agreement. For commercial redistribution and use, you are
* required to send a signed copy of the xvc License Agreement to Divideon.
*
* Redistribution and use in source and binary form is permitted free of charge
* for non-commercial purposes. See definition of non-commercial in the xvc
* License Agreement.
*
* All redistribution of source code must retain this copyright notice
* unmodified.
*
* The xvc License Agreement is available at https://xvc.io/license/.
******************************************************************************/

#ifndef XVC_ENC_LIB_SAMPLE_METRIC_H_
#define XVC_ENC_LIB_SAMPLE_METRIC_H_

#include <vector>

#include "xvc_common_lib/coding_unit.h"
#include "xvc_common_lib/common.h"
#include "xvc_common_lib/sample_buffer.h"
#include "xvc_common_lib/quantize.h"
#include "xvc_common_lib/yuv_pic.h"


namespace xvc {

enum class MetricType {
  kSsd,
  kSatd,
  kSad,
  kSadFast,
  kStructuralSsd,
};

class SampleMetric {
public:
  SampleMetric(MetricType type, const Qp &qp, int bitdepth)
    : type_(type), qp_(qp), bitdepth_(bitdepth) {
  }
  // Sample vs Sample
  Distortion CompareSample(const CodingUnit &cu, YuvComponent comp,
                           const YuvPicture &src1, const YuvPicture &src2);
  Distortion CompareSample(const CodingUnit &cu, YuvComponent comp,
                           const YuvPicture &src1,
                           const Sample *src2, ptrdiff_t stride2);
  Distortion CompareSample(const CodingUnit &cu, YuvComponent comp,
                           const YuvPicture &src1, const SampleBuffer &src2);
  Distortion CompareSample(const CodingUnit &cu, YuvComponent comp,
                           const SampleBuffer &src1,
                           const SampleBuffer &src2);
  Distortion CompareSample(YuvComponent comp, int width, int height,
                           const Sample *src1, ptrdiff_t stride1,
                           const Sample *src2, ptrdiff_t stride2);
  // Sample vs Residual
  Distortion CompareSample(YuvComponent comp, int width, int height,
                           const Residual *src1, ptrdiff_t stride1,
                           const Sample *src2, ptrdiff_t stride2);
  // Residual vs Residual
  Distortion CompareShort(YuvComponent comp, int width, int height,
                          const DataBuffer<Residual> &src1,
                          const DataBuffer<Residual> &src2);
  Distortion CompareShort(YuvComponent comp, int width, int height,
                          const Residual *src1, ptrdiff_t stride1,
                          const Residual *src2, ptrdiff_t stride2);

private:
  template<typename SampleT1, typename SampleT2>
  Distortion Compare(YuvComponent comp, int width, int height,
                     const SampleT1 *src1, ptrdiff_t stride1,
                     const SampleT2 *src2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeSsd(int width, int height,
                      const SampleT1 *sample1, ptrdiff_t stride1,
                      const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeSatd(int width, int height,
                       const SampleT1 *sample1, ptrdiff_t stride1,
                       const SampleT2 *sample2, ptrdiff_t stride2);
  template<int W, int H, typename SampleT1, typename SampleT2>
  int ComputeSatdNxM(const SampleT1 *sample1, ptrdiff_t stride1,
                     const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeSad(int width, int height,
                      const SampleT1 *sample1, ptrdiff_t stride1,
                      const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeSadFast(int width, int height,
                          const SampleT1 *sample1, ptrdiff_t stride1,
                          const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeStructuralSsd8(const SampleT1 *sample1, ptrdiff_t stride1,
                       const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeStructuralSsd4(const SampleT1 *sample1, ptrdiff_t stride1,
                        const SampleT2 *sample2, ptrdiff_t stride2);
  template<typename SampleT1, typename SampleT2>
  uint64_t ComputeStructuralSsd(int width, int height,
                       const SampleT1 *sample1, ptrdiff_t stride1,
                       const SampleT2 *sample2, ptrdiff_t stride2);

  MetricType type_;
  const Qp &qp_;
  int bitdepth_;
  std::vector<double> lambdas_;
};

}   // namespace xvc

#endif  // XVC_ENC_LIB_SAMPLE_METRIC_H_

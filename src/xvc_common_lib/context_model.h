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

#ifndef XVC_COMMON_LIB_CONTEXT_MODEL_H_
#define XVC_COMMON_LIB_CONTEXT_MODEL_H_

#include "xvc_common_lib/common.h"

namespace xvc {

class ContextModel {
public:
  static const int CONTEXT_STATE_BITS = 6;
  static const int kFracBitsPrecision = 15;
  static const uint32_t kEntropyBypassBits = 1 << kFracBitsPrecision;
  static uint32_t GetEntropyBitsTrm(uint32_t bin) {
    return kEntropyBits_[126 ^ bin];
  }

  ContextModel() : state_(0) {}
  void SetState(uint8_t state, uint8_t mps) { state_ = (state << 1) + mps; }
  void Init(int qp, int init_value);
  uint32_t GetState() const { return (state_ >> 1); }
  uint32_t GetMps() const { return (state_ & 1); }
  uint32_t GetEntropyBits(uint32_t bin) const {
    return kEntropyBits_[state_ ^ bin];
  }
  void UpdateLPS();
  void UpdateMPS();

private:
  static const int kNumTotalStates_ = 1 << (CONTEXT_STATE_BITS + 1);
  static const uint8_t kNextStateMps_[kNumTotalStates_];
  static const uint8_t kNextStateLps_[kNumTotalStates_];
  static const uint32_t kEntropyBits_[ContextModel::kNumTotalStates_];

  uint8_t state_;
};

}   // namespace xvc

#endif  // XVC_COMMON_LIB_CONTEXT_MODEL_H_

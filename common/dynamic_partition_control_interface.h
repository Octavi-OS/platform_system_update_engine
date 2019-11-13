//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_COMMON_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_
#define UPDATE_ENGINE_COMMON_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_

#include <stdint.h>

#include <memory>
#include <string>

#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

struct FeatureFlag {
  enum class Value { NONE = 0, RETROFIT, LAUNCH };
  constexpr explicit FeatureFlag(Value value) : value_(value) {}
  constexpr bool IsEnabled() const { return value_ != Value::NONE; }
  constexpr bool IsRetrofit() const { return value_ == Value::RETROFIT; }
  constexpr bool IsLaunch() const { return value_ == Value::LAUNCH; }

 private:
  Value value_;
};

class DynamicPartitionControlInterface {
 public:
  virtual ~DynamicPartitionControlInterface() = default;

  // Return the feature flags of dynamic partitions on this device.
  // Return RETROFIT iff dynamic partitions is retrofitted on this device,
  //        LAUNCH iff this device is launched with dynamic partitions,
  //        NONE iff dynamic partitions is disabled on this device.
  virtual FeatureFlag GetDynamicPartitionsFeatureFlag() = 0;

  // Return the feature flags of Virtual A/B on this device.
  virtual FeatureFlag GetVirtualAbFeatureFlag() = 0;

  // Do necessary cleanups before destroying the object.
  virtual void Cleanup() = 0;

  // Prepare all partitions for an update specified in |manifest|.
  // This is needed before calling MapPartitionOnDeviceMapper(), otherwise the
  // device would be mapped in an inconsistent way.
  // If |update| is set, create snapshots and writes super partition metadata.
  virtual bool PreparePartitionsForUpdate(uint32_t source_slot,
                                          uint32_t target_slot,
                                          const DeltaArchiveManifest& manifest,
                                          bool update) = 0;

  virtual bool FinishUpdate() = 0;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_
/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <wpi/StringMap.h>
#include <wpi/UidVector.h>
#include <wpi/spinlock.h>

#include "hal/Value.h"
#include "mockdata/SimCallbackRegistry.h"
#include "mockdata/SimDeviceData.h"

namespace hal {

namespace impl {

template <typename CallbackFunction>
class SimUnnamedCallbackRegistry {
 public:
  using RawFunctor = void (*)();

 protected:
  using CallbackVector = wpi::UidVector<HalCallbackListener<RawFunctor>, 4>;

 public:
  void Cancel(int32_t uid) {
    if (m_callbacks) m_callbacks->erase(uid - 1);
  }

  void Reset() {
    if (m_callbacks) m_callbacks->clear();
  }

  int32_t Register(CallbackFunction callback, void* param) {
    // Must return -1 on a null callback for error handling
    if (callback == nullptr) return -1;
    if (!m_callbacks) m_callbacks = std::make_unique<CallbackVector>();
    return m_callbacks->emplace_back(param,
                                     reinterpret_cast<RawFunctor>(callback)) +
           1;
  }

  template <typename... U>
  void Invoke(const char* name, U&&... u) const {
    if (m_callbacks) {
      for (auto&& cb : *m_callbacks) {
        reinterpret_cast<CallbackFunction>(cb.callback)(name, cb.param,
                                                        std::forward<U>(u)...);
      }
    }
  }

  template <typename... U>
  LLVM_ATTRIBUTE_ALWAYS_INLINE void operator()(U&&... u) const {
    return Invoke(std::forward<U>(u)...);
  }

 private:
  std::unique_ptr<CallbackVector> m_callbacks;
};

template <typename CallbackFunction>
class SimPrefixCallbackRegistry {
  struct CallbackData {
    CallbackData() = default;
    CallbackData(const char* prefix_, void* param_, CallbackFunction callback_)
        : prefix{prefix_}, callback{callback_}, param{param_} {}
    std::string prefix;
    CallbackFunction callback;
    void* param;

    explicit operator bool() const { return callback != nullptr; }
  };
  using CallbackVector = wpi::UidVector<CallbackData, 4>;

 public:
  void Cancel(int32_t uid) {
    if (m_callbacks) m_callbacks->erase(uid - 1);
  }

  void Reset() {
    if (m_callbacks) m_callbacks->clear();
  }

  int32_t Register(const char* prefix, void* param, CallbackFunction callback) {
    // Must return -1 on a null callback for error handling
    if (callback == nullptr) return -1;
    if (!m_callbacks) m_callbacks = std::make_unique<CallbackVector>();
    return m_callbacks->emplace_back(prefix, param, callback) + 1;
  }

  template <typename... U>
  void Invoke(const char* name, U&&... u) const {
    if (m_callbacks) {
      for (auto&& cb : *m_callbacks) {
        if (wpi::StringRef{name}.startswith(cb.prefix)) {
          cb.callback(name, cb.param, std::forward<U>(u)...);
        }
      }
    }
  }

  template <typename... U>
  LLVM_ATTRIBUTE_ALWAYS_INLINE void operator()(U&&... u) const {
    return Invoke(std::forward<U>(u)...);
  }

 private:
  std::unique_ptr<CallbackVector> m_callbacks;
};

}  // namespace impl

class SimDeviceData {
 private:
  struct Value {
    Value(const char* name_, bool readonly_, const HAL_Value& value_)
        : name{name_}, readonly{readonly_}, value{value_} {}

    HAL_SimValueHandle handle{0};
    std::string name;
    bool readonly;
    HAL_Value value;
    std::vector<std::string> enumOptions;
    std::vector<const char*> cstrEnumOptions;
    impl::SimUnnamedCallbackRegistry<HALSIM_SimValueCallback> changed;
  };

  struct Device {
    explicit Device(const char* name_) : name{name_} {}

    HAL_SimDeviceHandle handle{0};
    std::string name;
    wpi::UidVector<std::unique_ptr<Value>, 16> values;
    wpi::StringMap<Value*> valueMap;
    impl::SimUnnamedCallbackRegistry<HALSIM_SimValueCallback> valueCreated;
  };

  wpi::UidVector<std::shared_ptr<Device>, 4> m_devices;
  wpi::StringMap<std::weak_ptr<Device>> m_deviceMap;

  wpi::recursive_spinlock m_mutex;

  impl::SimPrefixCallbackRegistry<HALSIM_SimDeviceCallback> m_deviceCreated;
  impl::SimPrefixCallbackRegistry<HALSIM_SimDeviceCallback> m_deviceFreed;

  // call with lock held, returns null if does not exist
  Device* LookupDevice(HAL_SimDeviceHandle handle);
  Value* LookupValue(HAL_SimValueHandle handle);

 public:
  HAL_SimDeviceHandle CreateDevice(const char* name);
  void FreeDevice(HAL_SimDeviceHandle handle);
  HAL_SimValueHandle CreateValue(HAL_SimDeviceHandle device, const char* name,
                                 bool readonly, int32_t numOptions,
                                 const char** options,
                                 const HAL_Value& initialValue);
  HAL_Value GetValue(HAL_SimValueHandle handle);
  void SetValue(HAL_SimValueHandle handle, const HAL_Value& value);

  int32_t RegisterDeviceCreatedCallback(const char* prefix, void* param,
                                        HALSIM_SimDeviceCallback callback,
                                        bool initialNotify);

  void CancelDeviceCreatedCallback(int32_t uid);

  int32_t RegisterDeviceFreedCallback(const char* prefix, void* param,
                                      HALSIM_SimDeviceCallback callback);

  void CancelDeviceFreedCallback(int32_t uid);

  HAL_SimDeviceHandle GetDeviceHandle(const char* name);
  const char* GetDeviceName(HAL_SimDeviceHandle handle);

  void EnumerateDevices(const char* prefix, void* param,
                        HALSIM_SimDeviceCallback callback);

  int32_t RegisterValueCreatedCallback(HAL_SimDeviceHandle device, void* param,
                                       HALSIM_SimValueCallback callback,
                                       bool initialNotify);

  void CancelValueCreatedCallback(int32_t uid);

  int32_t RegisterValueChangedCallback(HAL_SimValueHandle handle, void* param,
                                       HALSIM_SimValueCallback callback,
                                       bool initialNotify);

  void CancelValueChangedCallback(int32_t uid);

  HAL_SimValueHandle GetValueHandle(HAL_SimDeviceHandle device,
                                    const char* name);

  void EnumerateValues(HAL_SimDeviceHandle device, void* param,
                       HALSIM_SimValueCallback callback);

  const char** GetValueEnumOptions(HAL_SimValueHandle handle,
                                   int32_t* numOptions);

  void ResetData();
};
extern SimDeviceData* SimSimDeviceData;
}  // namespace hal

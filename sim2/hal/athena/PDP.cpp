/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "hal/PDP.h"

#include <memory>

#include <wpi/mutex.h>

#include "HALInitializer.h"
#include "PortsInternal.h"
#include "hal/CANAPI.h"
#include "hal/Errors.h"
#include "hal/Ports.h"
#include "hal/handles/IndexedHandleResource.h"

using namespace hal;

static constexpr HAL_CANManufacturer manufacturer =
    HAL_CANManufacturer::HAL_CAN_Man_kCTRE;

static constexpr HAL_CANDeviceType deviceType =
    HAL_CANDeviceType::HAL_CAN_Dev_kPowerDistribution;

static constexpr int32_t Status1 = 0x50;
static constexpr int32_t Status2 = 0x51;
static constexpr int32_t Status3 = 0x52;
static constexpr int32_t StatusEnergy = 0x5D;

static constexpr int32_t Control1 = 0x70;

static constexpr int32_t TimeoutMs = 100;

/* encoder/decoders */
union PdpStatus1 {
  uint8_t data[8];
  struct Bits {
    unsigned chan1_h8 : 8;
    unsigned chan2_h6 : 6;
    unsigned chan1_l2 : 2;
    unsigned chan3_h4 : 4;
    unsigned chan2_l4 : 4;
    unsigned chan4_h2 : 2;
    unsigned chan3_l6 : 6;
    unsigned chan4_l8 : 8;
    unsigned chan5_h8 : 8;
    unsigned chan6_h6 : 6;
    unsigned chan5_l2 : 2;
    unsigned reserved4 : 4;
    unsigned chan6_l4 : 4;
  } bits;
};

union PdpStatus2 {
  uint8_t data[8];
  struct Bits {
    unsigned chan7_h8 : 8;
    unsigned chan8_h6 : 6;
    unsigned chan7_l2 : 2;
    unsigned chan9_h4 : 4;
    unsigned chan8_l4 : 4;
    unsigned chan10_h2 : 2;
    unsigned chan9_l6 : 6;
    unsigned chan10_l8 : 8;
    unsigned chan11_h8 : 8;
    unsigned chan12_h6 : 6;
    unsigned chan11_l2 : 2;
    unsigned reserved4 : 4;
    unsigned chan12_l4 : 4;
  } bits;
};

union PdpStatus3 {
  uint8_t data[8];
  struct Bits {
    unsigned chan13_h8 : 8;
    unsigned chan14_h6 : 6;
    unsigned chan13_l2 : 2;
    unsigned chan15_h4 : 4;
    unsigned chan14_l4 : 4;
    unsigned chan16_h2 : 2;
    unsigned chan15_l6 : 6;
    unsigned chan16_l8 : 8;
    unsigned internalResBattery_mOhms : 8;
    unsigned busVoltage : 8;
    unsigned temp : 8;
  } bits;
};

union PdpStatusEnergy {
  uint8_t data[8];
  struct Bits {
    unsigned TmeasMs_likelywillbe20ms_ : 8;
    unsigned TotalCurrent_125mAperunit_h8 : 8;
    unsigned Power_125mWperunit_h4 : 4;
    unsigned TotalCurrent_125mAperunit_l4 : 4;
    unsigned Power_125mWperunit_m8 : 8;
    unsigned Energy_125mWPerUnitXTmeas_h4 : 4;
    unsigned Power_125mWperunit_l4 : 4;
    unsigned Energy_125mWPerUnitXTmeas_mh8 : 8;
    unsigned Energy_125mWPerUnitXTmeas_ml8 : 8;
    unsigned Energy_125mWPerUnitXTmeas_l8 : 8;
  } bits;
};

static wpi::mutex pdpHandleMutex;
static HAL_PDPHandle pdpHandles[kNumPDPModules];

namespace hal {
namespace init {
void InitializePDP() {
  for (int i = 0; i < kNumPDPModules; i++) {
    pdpHandles[i] = HAL_kInvalidHandle;
  }
}
}  // namespace init
}  // namespace hal

extern "C" {

HAL_PDPHandle HAL_InitializePDP(int32_t module, int32_t* status) {
  hal::init::CheckInit();
  if (!HAL_CheckPDPModule(module)) {
    *status = PARAMETER_OUT_OF_RANGE;
    return HAL_kInvalidHandle;
  }

  std::scoped_lock lock(pdpHandleMutex);

  if (pdpHandles[module] != HAL_kInvalidHandle) {
    *status = 0;
    return pdpHandles[module];
  }

  auto handle = HAL_InitializeCAN(manufacturer, module, deviceType, status);

  if (*status != 0) {
    HAL_CleanCAN(handle);
    return HAL_kInvalidHandle;
  }

  pdpHandles[module] = handle;

  return handle;
}

void HAL_CleanPDP(HAL_PDPHandle handle) {
  HAL_CleanCAN(handle);

  for (int i = 0; i < kNumPDPModules; i++) {
    if (pdpHandles[i] == handle) {
      pdpHandles[i] = HAL_kInvalidHandle;
      return;
    }
  }
}

HAL_Bool HAL_CheckPDPModule(int32_t module) {
  return module < kNumPDPModules && module >= 0;
}

HAL_Bool HAL_CheckPDPChannel(int32_t channel) {
  return channel < kNumPDPChannels && channel >= 0;
}

double HAL_GetPDPTemperature(HAL_PDPHandle handle, int32_t* status) {
  PdpStatus3 pdpStatus;
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  HAL_ReadCANPacketTimeout(handle, Status3, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);

  if (*status != 0) {
    return 0;
  } else {
    return pdpStatus.bits.temp * 1.03250836957542 - 67.8564500484966;
  }
}

double HAL_GetPDPVoltage(HAL_PDPHandle handle, int32_t* status) {
  PdpStatus3 pdpStatus;
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  HAL_ReadCANPacketTimeout(handle, Status3, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);

  if (*status != 0) {
    return 0;
  } else {
    return pdpStatus.bits.busVoltage * 0.05 + 4.0; /* 50mV per unit plus 4V. */
  }
}

double HAL_GetPDPChannelCurrent(HAL_PDPHandle handle, int32_t channel,
                                int32_t* status) {
  if (!HAL_CheckPDPChannel(channel)) {
    *status = PARAMETER_OUT_OF_RANGE;
    return 0;
  }

  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  double raw = 0;

  if (channel <= 5) {
    PdpStatus1 pdpStatus;
    HAL_ReadCANPacketTimeout(handle, Status1, pdpStatus.data, &length,
                             &receivedTimestamp, TimeoutMs, status);
    if (*status != 0) {
      return 0;
    }
    switch (channel) {
      case 0:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan1_h8) << 2) |
              pdpStatus.bits.chan1_l2;
        break;
      case 1:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan2_h6) << 4) |
              pdpStatus.bits.chan2_l4;
        break;
      case 2:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan3_h4) << 6) |
              pdpStatus.bits.chan3_l6;
        break;
      case 3:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan4_h2) << 8) |
              pdpStatus.bits.chan4_l8;
        break;
      case 4:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan5_h8) << 2) |
              pdpStatus.bits.chan5_l2;
        break;
      case 5:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan6_h6) << 4) |
              pdpStatus.bits.chan6_l4;
        break;
    }
  } else if (channel <= 11) {
    PdpStatus2 pdpStatus;
    HAL_ReadCANPacketTimeout(handle, Status2, pdpStatus.data, &length,
                             &receivedTimestamp, TimeoutMs, status);
    if (*status != 0) {
      return 0;
    }
    switch (channel) {
      case 6:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan7_h8) << 2) |
              pdpStatus.bits.chan7_l2;
        break;
      case 7:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan8_h6) << 4) |
              pdpStatus.bits.chan8_l4;
        break;
      case 8:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan9_h4) << 6) |
              pdpStatus.bits.chan9_l6;
        break;
      case 9:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan10_h2) << 8) |
              pdpStatus.bits.chan10_l8;
        break;
      case 10:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan11_h8) << 2) |
              pdpStatus.bits.chan11_l2;
        break;
      case 11:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan12_h6) << 4) |
              pdpStatus.bits.chan12_l4;
        break;
    }
  } else {
    PdpStatus3 pdpStatus;
    HAL_ReadCANPacketTimeout(handle, Status3, pdpStatus.data, &length,
                             &receivedTimestamp, TimeoutMs, status);
    if (*status != 0) {
      return 0;
    }
    switch (channel) {
      case 12:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan13_h8) << 2) |
              pdpStatus.bits.chan13_l2;
        break;
      case 13:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan14_h6) << 4) |
              pdpStatus.bits.chan14_l4;
        break;
      case 14:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan15_h4) << 6) |
              pdpStatus.bits.chan15_l6;
        break;
      case 15:
        raw = (static_cast<uint32_t>(pdpStatus.bits.chan16_h2) << 8) |
              pdpStatus.bits.chan16_l8;
        break;
    }
  }

  /* convert to amps */
  return raw * 0.125; /* 7.3 fixed pt value in Amps */
}

void HAL_GetPDPAllChannelCurrents(HAL_PDPHandle handle, double* currents,
                                  int32_t* status) {
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;
  PdpStatus1 pdpStatus;
  HAL_ReadCANPacketTimeout(handle, Status1, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) return;
  PdpStatus2 pdpStatus2;
  HAL_ReadCANPacketTimeout(handle, Status2, pdpStatus2.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) return;
  PdpStatus3 pdpStatus3;
  HAL_ReadCANPacketTimeout(handle, Status3, pdpStatus3.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) return;

  currents[0] = ((static_cast<uint32_t>(pdpStatus.bits.chan1_h8) << 2) |
                 pdpStatus.bits.chan1_l2) *
                0.125;
  currents[1] = ((static_cast<uint32_t>(pdpStatus.bits.chan2_h6) << 4) |
                 pdpStatus.bits.chan2_l4) *
                0.125;
  currents[2] = ((static_cast<uint32_t>(pdpStatus.bits.chan3_h4) << 6) |
                 pdpStatus.bits.chan3_l6) *
                0.125;
  currents[3] = ((static_cast<uint32_t>(pdpStatus.bits.chan4_h2) << 8) |
                 pdpStatus.bits.chan4_l8) *
                0.125;
  currents[4] = ((static_cast<uint32_t>(pdpStatus.bits.chan5_h8) << 2) |
                 pdpStatus.bits.chan5_l2) *
                0.125;
  currents[5] = ((static_cast<uint32_t>(pdpStatus.bits.chan6_h6) << 4) |
                 pdpStatus.bits.chan6_l4) *
                0.125;

  currents[6] = ((static_cast<uint32_t>(pdpStatus2.bits.chan7_h8) << 2) |
                 pdpStatus2.bits.chan7_l2) *
                0.125;
  currents[7] = ((static_cast<uint32_t>(pdpStatus2.bits.chan8_h6) << 4) |
                 pdpStatus2.bits.chan8_l4) *
                0.125;
  currents[8] = ((static_cast<uint32_t>(pdpStatus2.bits.chan9_h4) << 6) |
                 pdpStatus2.bits.chan9_l6) *
                0.125;
  currents[9] = ((static_cast<uint32_t>(pdpStatus2.bits.chan10_h2) << 8) |
                 pdpStatus2.bits.chan10_l8) *
                0.125;
  currents[10] = ((static_cast<uint32_t>(pdpStatus2.bits.chan11_h8) << 2) |
                  pdpStatus2.bits.chan11_l2) *
                 0.125;
  currents[11] = ((static_cast<uint32_t>(pdpStatus2.bits.chan12_h6) << 4) |
                  pdpStatus2.bits.chan12_l4) *
                 0.125;

  currents[12] = ((static_cast<uint32_t>(pdpStatus3.bits.chan13_h8) << 2) |
                  pdpStatus3.bits.chan13_l2) *
                 0.125;
  currents[13] = ((static_cast<uint32_t>(pdpStatus3.bits.chan14_h6) << 4) |
                  pdpStatus3.bits.chan14_l4) *
                 0.125;
  currents[14] = ((static_cast<uint32_t>(pdpStatus3.bits.chan15_h4) << 6) |
                  pdpStatus3.bits.chan15_l6) *
                 0.125;
  currents[15] = ((static_cast<uint32_t>(pdpStatus3.bits.chan16_h2) << 8) |
                  pdpStatus3.bits.chan16_l8) *
                 0.125;
}

double HAL_GetPDPTotalCurrent(HAL_PDPHandle handle, int32_t* status) {
  PdpStatusEnergy pdpStatus;
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  HAL_ReadCANPacketTimeout(handle, StatusEnergy, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) {
    return 0;
  }

  uint32_t raw;
  raw = pdpStatus.bits.TotalCurrent_125mAperunit_h8;
  raw <<= 4;
  raw |= pdpStatus.bits.TotalCurrent_125mAperunit_l4;
  return 0.125 * raw; /* 7.3 fixed pt value in Amps */
}

double HAL_GetPDPTotalPower(HAL_PDPHandle handle, int32_t* status) {
  PdpStatusEnergy pdpStatus;
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  HAL_ReadCANPacketTimeout(handle, StatusEnergy, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) {
    return 0;
  }

  uint32_t raw;
  raw = pdpStatus.bits.Power_125mWperunit_h4;
  raw <<= 8;
  raw |= pdpStatus.bits.Power_125mWperunit_m8;
  raw <<= 4;
  raw |= pdpStatus.bits.Power_125mWperunit_l4;
  return 0.125 * raw; /* 7.3 fixed pt value in Watts */
}

double HAL_GetPDPTotalEnergy(HAL_PDPHandle handle, int32_t* status) {
  PdpStatusEnergy pdpStatus;
  int32_t length = 0;
  uint64_t receivedTimestamp = 0;

  HAL_ReadCANPacketTimeout(handle, StatusEnergy, pdpStatus.data, &length,
                           &receivedTimestamp, TimeoutMs, status);
  if (*status != 0) {
    return 0;
  }

  uint32_t raw;
  raw = pdpStatus.bits.Energy_125mWPerUnitXTmeas_h4;
  raw <<= 8;
  raw |= pdpStatus.bits.Energy_125mWPerUnitXTmeas_mh8;
  raw <<= 8;
  raw |= pdpStatus.bits.Energy_125mWPerUnitXTmeas_ml8;
  raw <<= 8;
  raw |= pdpStatus.bits.Energy_125mWPerUnitXTmeas_l8;

  double energyJoules = 0.125 * raw; /* mW integrated every TmeasMs */
  energyJoules *= 0.001;             /* convert from mW to W */
  energyJoules *=
      pdpStatus.bits
          .TmeasMs_likelywillbe20ms_; /* multiplied by TmeasMs = joules */
  return 0.125 * raw;
}

void HAL_ResetPDPTotalEnergy(HAL_PDPHandle handle, int32_t* status) {
  uint8_t pdpControl[] = {0x40}; /* only bit set is ResetEnergy */
  HAL_WriteCANPacket(handle, pdpControl, 1, Control1, status);
}

void HAL_ClearPDPStickyFaults(HAL_PDPHandle handle, int32_t* status) {
  uint8_t pdpControl[] = {0x80}; /* only bit set is ClearStickyFaults */
  HAL_WriteCANPacket(handle, pdpControl, 1, Control1, status);
}

}  // extern "C"

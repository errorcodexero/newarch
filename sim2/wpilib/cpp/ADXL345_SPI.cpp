/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/ADXL345_SPI.h"

#include <hal/FRCUsageReporting.h>

#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"

using namespace frc;

ADXL345_SPI::ADXL345_SPI(SPI::Port port, ADXL345_SPI::Range range)
    : m_spi(port), m_simDevice("ADXL345_SPI", port) {
  if (m_simDevice) {
    m_simRange =
        m_simDevice.CreateEnum("Range", true, {"2G", "4G", "8G", "16G"}, 0);
    m_simX = m_simDevice.CreateDouble("X Accel", false, 0.0);
    m_simX = m_simDevice.CreateDouble("Y Accel", false, 0.0);
    m_simZ = m_simDevice.CreateDouble("Z Accel", false, 0.0);
  }
  m_spi.SetClockRate(500000);
  m_spi.SetMSBFirst();
  m_spi.SetSampleDataOnTrailingEdge();
  m_spi.SetClockActiveLow();
  m_spi.SetChipSelectActiveHigh();

  uint8_t commands[2];
  // Turn on the measurements
  commands[0] = kPowerCtlRegister;
  commands[1] = kPowerCtl_Measure;
  m_spi.Transaction(commands, commands, 2);

  SetRange(range);

  HAL_Report(HALUsageReporting::kResourceType_ADXL345,
             HALUsageReporting::kADXL345_SPI);

  SendableRegistry::GetInstance().AddLW(this, "ADXL345_SPI", port);
}

void ADXL345_SPI::SetRange(Range range) {
  uint8_t commands[2];

  // Specify the data format to read
  commands[0] = kDataFormatRegister;
  commands[1] = kDataFormat_FullRes | static_cast<uint8_t>(range & 0x03);
  m_spi.Transaction(commands, commands, 2);

  if (m_simRange) m_simRange.Set(range);
}

double ADXL345_SPI::GetX() { return GetAcceleration(kAxis_X); }

double ADXL345_SPI::GetY() { return GetAcceleration(kAxis_Y); }

double ADXL345_SPI::GetZ() { return GetAcceleration(kAxis_Z); }

double ADXL345_SPI::GetAcceleration(ADXL345_SPI::Axes axis) {
  if (axis == kAxis_X && m_simX) return m_simX.Get();
  if (axis == kAxis_Y && m_simY) return m_simY.Get();
  if (axis == kAxis_Z && m_simZ) return m_simZ.Get();
  uint8_t buffer[3];
  uint8_t command[3] = {0, 0, 0};
  command[0] = (kAddress_Read | kAddress_MultiByte | kDataRegister) +
               static_cast<uint8_t>(axis);
  m_spi.Transaction(command, buffer, 3);

  // Sensor is little endian... swap bytes
  int16_t rawAccel = buffer[2] << 8 | buffer[1];
  return rawAccel * kGsPerLSB;
}

ADXL345_SPI::AllAxes ADXL345_SPI::GetAccelerations() {
  AllAxes data;
  if (m_simX && m_simY && m_simZ) {
    data.XAxis = m_simX.Get();
    data.YAxis = m_simY.Get();
    data.ZAxis = m_simZ.Get();
    return data;
  }

  uint8_t dataBuffer[7] = {0, 0, 0, 0, 0, 0, 0};
  int16_t rawData[3];

  // Select the data address.
  dataBuffer[0] = (kAddress_Read | kAddress_MultiByte | kDataRegister);
  m_spi.Transaction(dataBuffer, dataBuffer, 7);

  for (int i = 0; i < 3; i++) {
    // Sensor is little endian... swap bytes
    rawData[i] = dataBuffer[i * 2 + 2] << 8 | dataBuffer[i * 2 + 1];
  }

  data.XAxis = rawData[0] * kGsPerLSB;
  data.YAxis = rawData[1] * kGsPerLSB;
  data.ZAxis = rawData[2] * kGsPerLSB;

  return data;
}

void ADXL345_SPI::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("3AxisAccelerometer");
  auto x = builder.GetEntry("X").GetHandle();
  auto y = builder.GetEntry("Y").GetHandle();
  auto z = builder.GetEntry("Z").GetHandle();
  builder.SetUpdateTable([=]() {
    auto data = GetAccelerations();
    nt::NetworkTableEntry(x).SetDouble(data.XAxis);
    nt::NetworkTableEntry(y).SetDouble(data.YAxis);
    nt::NetworkTableEntry(z).SetDouble(data.ZAxis);
  });
}

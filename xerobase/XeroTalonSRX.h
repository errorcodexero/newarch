#ifdef ENABLE_SIMULATOR
#include <TalonSRX.h>
#include <SensorCollection.h>
#else
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#include <ctre/phoenix/MotorControl/SensorCollection.h>
#endif

/// \file

/// \brief convenience definint
typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

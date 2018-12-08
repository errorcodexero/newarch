#ifdef SIM
#include <TalonSRX.h>
#else
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#endif

/// \file

/// \brief convenience definint
typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;
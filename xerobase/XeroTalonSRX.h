#ifdef SIM
#include <TalonSRX.h>
#else
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#endif

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;
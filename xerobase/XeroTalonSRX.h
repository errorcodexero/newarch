#ifdef SIMULATOR
#include <TalonSRX.h>
#include <SensorCollection.h>
#else
#include "ctre/Phoenix.h"
#endif

/// \file

/// \brief convenience definint
typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

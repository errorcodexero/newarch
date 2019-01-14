#pragma once

#include <wpi/mutex.h>
#include <wpi/Twine.h>
#include <cstdint>

#define NullParameter                           (1)
#define SmartDashboardMissingKey                (2)

inline void wpi_setGlobalWPIErrorWithContext(uint32_t,  wpi::StringRef str) {
}




namespace frc
{
    class ErrorBase
    {

    } ;
}

#include <hal/Power.h>

double HAL_GetVinVoltage(int32_t* status)
{
    return 12.0;
}

double HAL_GetVinCurrent(int32_t* status)
{
    return 0.0;
}

double HAL_GetUserVoltage6V(int32_t* status)
{
    return 0.0;
}

double HAL_GetUserCurrent6V(int32_t* status)
{
    return 0.0;
}

HAL_Bool HAL_GetUserActive6V(int32_t* status)
{
    return 1;
}

int32_t HAL_GetUserCurrentFaults6V(int32_t* status)
{
    return 0;
}

double HAL_GetUserVoltage5V(int32_t* status)
{
    return 0.0;
}
double HAL_GetUserCurrent5V(int32_t* status)
{
    return 0.0;
}
HAL_Bool HAL_GetUserActive5V(int32_t* status)
{
    return 1;
}
int32_t HAL_GetUserCurrentFaults5V(int32_t* status)
{
    return 0;
}
double HAL_GetUserVoltage3V3(int32_t* status)
{
    return 0.0;
}
double HAL_GetUserCurrent3V3(int32_t* status)
{
    return 0.0;
}
HAL_Bool HAL_GetUserActive3V3(int32_t* status)
{
    return 1;
}
int32_t HAL_GetUserCurrentFaults3V3(int32_t* status)
{
    return 0;
}

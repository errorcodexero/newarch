#include <hal/Ports.h>
#include <hal/Solenoid.h>

int32_t HAL_GetNumSolenoidChannels()
{
    return 0;
}

int32_t HAL_GetNumPCMModules()
{
    return 0;
}

HAL_SolenoidHandle HAL_InitializeSolenoidPort(HAL_PortHandle portHandle,
                                              int32_t* status)
{
    return 0;
}
void HAL_FreeSolenoidPort(HAL_SolenoidHandle solenoidPortHandle)
{
}
HAL_Bool HAL_CheckSolenoidModule(int32_t module)
{
    return 0;
}
HAL_Bool HAL_CheckSolenoidChannel(int32_t channel)
{
    return 0;
}
HAL_Bool HAL_GetSolenoid(HAL_SolenoidHandle solenoidPortHandle,
                         int32_t* status)
{
    return 0;
}
int32_t HAL_GetAllSolenoids(int32_t module, int32_t* status)
{
    return 0;
}
void HAL_SetSolenoid(HAL_SolenoidHandle solenoidPortHandle, HAL_Bool value,
                     int32_t* status)
{

}
void HAL_SetAllSolenoids(int32_t module, int32_t state, int32_t* status)
{

}
int32_t HAL_GetPCMSolenoidBlackList(int32_t module, int32_t* status)
{
    return 0;
}
HAL_Bool HAL_GetPCMSolenoidVoltageStickyFault(int32_t module, int32_t* status)
{
    return 1;
}
HAL_Bool HAL_GetPCMSolenoidVoltageFault(int32_t module, int32_t* status)
{
    return 1;
}
void HAL_ClearAllPCMStickyFaults(int32_t module, int32_t* status)
{

}

void HAL_SetOneShotDuration(HAL_SolenoidHandle solenoidPortHandle, int32_t durMS, int32_t* status)
{

}

void HAL_FireOneShot(HAL_SolenoidHandle solenoidPortHandle, int32_t* status)
{

}

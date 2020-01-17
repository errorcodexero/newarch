#include <hal/PDP.h>
#include <hal/Ports.h>

int32_t HAL_GetNumPDPChannels()
{
    return 0;
}

HAL_PDPHandle HAL_InitializePDP(int32_t module, int32_t* status)
{
    return 0;
}

void HAL_CleanPDP(HAL_PDPHandle handle)
{

}

HAL_Bool HAL_CheckPDPChannel(int32_t channel)
{
    return 1;
}

HAL_Bool HAL_CheckPDPModule(int32_t module)
{
    return 1;
}

double HAL_GetPDPTemperature(HAL_PDPHandle handle, int32_t* status)
{
    return 0.0;
}

double HAL_GetPDPVoltage(HAL_PDPHandle handle, int32_t* status)
{
    return 0.0;
}

double HAL_GetPDPChannelCurrent(HAL_PDPHandle handle, int32_t channel,
                                int32_t* status)
{
    return 0.0 ;
}

void HAL_GetPDPAllChannelCurrents(HAL_PDPHandle handle, double* currents,
                                  int32_t* status)
{

}

double HAL_GetPDPTotalCurrent(HAL_PDPHandle handle, int32_t* status)
{
    return 0.0;
}

double HAL_GetPDPTotalPower(HAL_PDPHandle handle, int32_t* status)
{
    return 0.0;
}

double HAL_GetPDPTotalEnergy(HAL_PDPHandle handle, int32_t* status)
{
    return 0.0;
}

void HAL_ResetPDPTotalEnergy(HAL_PDPHandle handle, int32_t* status)
{
}

void HAL_ClearPDPStickyFaults(HAL_PDPHandle handle, int32_t* status)
{
}

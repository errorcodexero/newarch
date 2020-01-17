#include <hal/AnalogOutput.h>
#include <hal/Ports.h>

HAL_AnalogOutputHandle HAL_InitializeAnalogOutputPort(HAL_PortHandle portHandle,
                                                      int32_t* status)
{
    return 0;
}

void HAL_FreeAnalogOutputPort(HAL_AnalogOutputHandle analogOutputHandle)
{

}

void HAL_SetAnalogOutput(HAL_AnalogOutputHandle analogOutputHandle,
                         double voltage, int32_t* status)
{

}

double HAL_GetAnalogOutput(HAL_AnalogOutputHandle analogOutputHandle,
                           int32_t* status)
{
    return 0.0;
}                           

HAL_Bool HAL_CheckAnalogOutputChannel(int32_t channel)
{
    return 1;
}
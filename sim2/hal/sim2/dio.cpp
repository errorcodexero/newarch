#include <hal/Ports.h>
#include <hal/DIO.h>

int32_t HAL_GetNumDigitalChannels()
{
    return 0;
}

HAL_DigitalHandle HAL_InitializeDIOPort(HAL_PortHandle portHandle,
                                        HAL_Bool input, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_CheckDIOChannel(int32_t channel)
{
    return 0;
}

void HAL_FreeDIOPort(HAL_DigitalHandle dioPortHandle)
{

}

void HAL_SetDIOSimDevice(HAL_DigitalHandle handle, HAL_SimDeviceHandle device)
{

}

HAL_DigitalPWMHandle HAL_AllocateDigitalPWM(int32_t* status)
{
    return 0;
}

void HAL_FreeDigitalPWM(HAL_DigitalPWMHandle pwmGenerator, int32_t* status)
{

}

void HAL_SetDigitalPWMRate(double rate, int32_t* status)
{

}

void HAL_SetDigitalPWMDutyCycle(HAL_DigitalPWMHandle pwmGenerator,
                                double dutyCycle, int32_t* status)
{
}                                

void HAL_SetDigitalPWMOutputChannel(HAL_DigitalPWMHandle pwmGenerator,
                                    int32_t channel, int32_t* status)
{

}

void HAL_SetDIO(HAL_DigitalHandle dioPortHandle, HAL_Bool value,
                int32_t* status)
{

}

void HAL_SetDIODirection(HAL_DigitalHandle dioPortHandle, HAL_Bool input,
                         int32_t* status)
{

}

HAL_Bool HAL_GetDIO(HAL_DigitalHandle dioPortHandle, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_GetDIODirection(HAL_DigitalHandle dioPortHandle, int32_t* status)
{
    return 0;
}

void HAL_Pulse(HAL_DigitalHandle dioPortHandle, double pulseLength,
               int32_t* status)
{

}

HAL_Bool HAL_IsPulsing(HAL_DigitalHandle dioPortHandle, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_IsAnyPulsing(int32_t* status)
{
    return 0;
}

void HAL_SetFilterSelect(HAL_DigitalHandle dioPortHandle, int32_t filterIndex,
                         int32_t* status)
{

}

int32_t HAL_GetFilterSelect(HAL_DigitalHandle dioPortHandle, int32_t* status)
{
    return 0;
}

void HAL_SetFilterPeriod(int32_t filterIndex, int64_t value, int32_t* status)
{

}

int64_t HAL_GetFilterPeriod(int32_t filterIndex, int32_t* status)
{
    return 0;
}

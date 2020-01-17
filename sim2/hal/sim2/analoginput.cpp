#include <hal/AnalogInput.h>
#include <hal/Ports.h>

int32_t HAL_GetNumAnalogInputs()
{
    return 8;
}

HAL_AnalogInputHandle HAL_InitializeAnalogInputPort(HAL_PortHandle portHandle, int32_t *status)
{
    return 0;
}

void HAL_FreeAnalogInputPort(HAL_AnalogInputHandle analogPortHandle)
{
}

HAL_Bool HAL_CheckAnalogModule(int32_t module)
{
    return 1;
}

HAL_Bool HAL_CheckAnalogInputChannel(int32_t channel)
{
    return 1;
}

void HAL_SetAnalogInputSimDevice(HAL_AnalogInputHandle handle, HAL_SimDeviceHandle device)
{
}

void HAL_SetAnalogSampleRate(double samplesPerSecond, int32_t* status)
{
}

double HAL_GetAnalogSampleRate(int32_t* status)
{
    return 0.0;
}

void HAL_SetAnalogAverageBits(HAL_AnalogInputHandle analogPortHandle,
                              int32_t bits, int32_t* status)
{
}

int32_t HAL_GetAnalogAverageBits(HAL_AnalogInputHandle analogPortHandle,
                                 int32_t* status)
{
    return 0;
}

void HAL_SetAnalogOversampleBits(HAL_AnalogInputHandle analogPortHandle,
                                 int32_t bits, int32_t* status)
{

}

int32_t HAL_GetAnalogOversampleBits(HAL_AnalogInputHandle analogPortHandle,
                                    int32_t* status)
{
    return 0;
}

int32_t HAL_GetAnalogValue(HAL_AnalogInputHandle analogPortHandle,
                           int32_t* status)
{
    return 0;
}

int32_t HAL_GetAnalogAverageValue(HAL_AnalogInputHandle analogPortHandle,
                                  int32_t* status)
{
    return 0;
}

int32_t HAL_GetAnalogVoltsToValue(HAL_AnalogInputHandle analogPortHandle,
                                  double voltage, int32_t* status)
{
    return 0;
}

double HAL_GetAnalogVoltage(HAL_AnalogInputHandle analogPortHandle,
                            int32_t* status)
{
    return 0.0;
}

double HAL_GetAnalogAverageVoltage(HAL_AnalogInputHandle analogPortHandle,
                                   int32_t* status)
{
    return 0.0;
}

int32_t HAL_GetAnalogLSBWeight(HAL_AnalogInputHandle analogPortHandle,
                               int32_t* status)
{
    return 0;
}

int32_t HAL_GetAnalogOffset(HAL_AnalogInputHandle analogPortHandle,
                            int32_t* status)
{
    return 0;
}

double HAL_GetAnalogValueToVolts(HAL_AnalogInputHandle analogPortHandle,
                                 int32_t rawValue, int32_t* status)
{
    return 0.0;
}

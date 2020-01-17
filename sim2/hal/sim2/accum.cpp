#include <hal/Ports.h>
#include <hal/AnalogAccumulator.h>

HAL_Bool HAL_IsAccumulatorChannel(HAL_AnalogInputHandle analogPortHandle, int32_t* status)
{
    return 0;
}

void HAL_InitAccumulator(HAL_AnalogInputHandle analogPortHandle, int32_t* status)
{
}                         

void HAL_ResetAccumulator(HAL_AnalogInputHandle analogPortHandle, int32_t* status)
{    
}

void HAL_SetAccumulatorCenter(HAL_AnalogInputHandle analogPortHandle, int32_t center, int32_t* status)
{
}

void HAL_SetAccumulatorDeadband(HAL_AnalogInputHandle analogPortHandle,
                                int32_t deadband, int32_t* status)
{
}

int64_t HAL_GetAccumulatorValue(HAL_AnalogInputHandle analogPortHandle,
                                int32_t* status)
{
    return 0;
}

int64_t HAL_GetAccumulatorCount(HAL_AnalogInputHandle analogPortHandle,
                                int32_t* status)
{
    return 0;
}

void HAL_GetAccumulatorOutput(HAL_AnalogInputHandle analogPortHandle,
                              int64_t* value, int64_t* count, int32_t* status)
{
}

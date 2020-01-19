#include <hal/AnalogTrigger.h>
#include <hal/Ports.h>

HAL_AnalogTriggerHandle HAL_InitializeAnalogTrigger(
    HAL_AnalogInputHandle portHandle, int32_t* status)
{
    return 0;
}

HAL_AnalogTriggerHandle HAL_InitializeAnalogTriggerDutyCycle(
    HAL_DutyCycleHandle dutyCycleHandle, int32_t* status)
{
    return 0;
}

void HAL_CleanAnalogTrigger(HAL_AnalogTriggerHandle analogTriggerHandle,
                            int32_t* status)
{

}

void HAL_SetAnalogTriggerLimitsRaw(HAL_AnalogTriggerHandle analogTriggerHandle,
                                   int32_t lower, int32_t upper,
                                   int32_t* status)
{

}                                   

void HAL_SetAnalogTriggerLimitsVoltage(
    HAL_AnalogTriggerHandle analogTriggerHandle, double lower, double upper,
    int32_t* status)
{

}    

void HAL_SetAnalogTriggerLimitsDutyCycle(
    HAL_AnalogTriggerHandle analogTriggerHandle, double lower, double upper,
    int32_t* status)
{

}

void HAL_SetAnalogTriggerAveraged(HAL_AnalogTriggerHandle analogTriggerHandle,
                                  HAL_Bool useAveragedValue, int32_t* status)
{

}

void HAL_SetAnalogTriggerFiltered(HAL_AnalogTriggerHandle analogTriggerHandle,
                                  HAL_Bool useFilteredValue, int32_t* status)
{

}

HAL_Bool HAL_GetAnalogTriggerInWindow(
    HAL_AnalogTriggerHandle analogTriggerHandle, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_GetAnalogTriggerTriggerState(
    HAL_AnalogTriggerHandle analogTriggerHandle, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_GetAnalogTriggerOutput(HAL_AnalogTriggerHandle analogTriggerHandle,
                                    HAL_AnalogTriggerType type,
                                    int32_t* status)
{
    return 0;
}

int32_t HAL_GetAnalogTriggerFPGAIndex(
    HAL_AnalogTriggerHandle analogTriggerHandle, int32_t* status)
{
    return 0;
}    

#include <hal/Encoder.h>

HAL_EncoderHandle HAL_InitializeEncoder(
    HAL_Handle digitalSourceHandleA, HAL_AnalogTriggerType analogTriggerTypeA,
    HAL_Handle digitalSourceHandleB, HAL_AnalogTriggerType analogTriggerTypeB,
    HAL_Bool reverseDirection, HAL_EncoderEncodingType encodingType,
    int32_t* status)
{
    return 0;
}

void HAL_FreeEncoder(HAL_EncoderHandle encoderHandle, int32_t* status)
{

}

void HAL_SetEncoderSimDevice(HAL_EncoderHandle handle, HAL_SimDeviceHandle device)
{

}

int32_t HAL_GetEncoder(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0;
}

int32_t HAL_GetEncoderRaw(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0;
}

int32_t HAL_GetEncoderEncodingScale(HAL_EncoderHandle encoderHandle, int32_t *status)
{
    return 0;
}

void HAL_ResetEncoder(HAL_EncoderHandle encoderHandle, int32_t* status)
{
}

double HAL_GetEncoderPeriod(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0.0;
}

void HAL_SetEncoderMaxPeriod(HAL_EncoderHandle encoderHandle, double maxPeriod, int32_t* status)
{

}

HAL_Bool HAL_GetEncoderStopped(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0;
}

HAL_Bool HAL_GetEncoderDirection(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0;
}

double HAL_GetEncoderDistance(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0.0;
}

double HAL_GetEncoderRate(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0.0;
}

void HAL_SetEncoderMinRate(HAL_EncoderHandle encoderHandle, double minRate, int32_t* status)
{
}

void HAL_SetEncoderDistancePerPulse(HAL_EncoderHandle encoderHandle, double distancePerPulse, int32_t* status)
{

}

void HAL_SetEncoderReverseDirection(HAL_EncoderHandle encoderHandle, HAL_Bool reverseDirection, int32_t* status)
{

}

void HAL_SetEncoderSamplesToAverage(HAL_EncoderHandle encoderHandle, int32_t samplesToAverage, int32_t* status)
{

}

int32_t HAL_GetEncoderSamplesToAverage(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return 0;
}

void HAL_SetEncoderIndexSource(HAL_EncoderHandle encoderHandle,
                               HAL_Handle digitalSourceHandle,
                               HAL_AnalogTriggerType analogTriggerType,
                               HAL_EncoderIndexingType type, int32_t* status)
{

}

int32_t HAL_GetEncoderFPGAIndex(HAL_EncoderHandle encoderHandle,
                                int32_t* status)
{
    return 0;
}

double HAL_GetEncoderDecodingScaleFactor(HAL_EncoderHandle encoderHandle,
                                         int32_t* status)
{
    return 0.0;
}

double HAL_GetEncoderDistancePerPulse(HAL_EncoderHandle encoderHandle,
                                      int32_t* status)
{
    return 0.0;
}                                      

HAL_EncoderEncodingType HAL_GetEncoderEncodingType(HAL_EncoderHandle encoderHandle, int32_t* status)
{
    return HAL_Encoder_k4X;
}

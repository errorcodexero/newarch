#include <hal/Interrupts.h>

HAL_InterruptHandle HAL_InitializeInterrupts(HAL_Bool watcher, int32_t* status)
{
    return 0;
}

void* HAL_CleanInterrupts(HAL_InterruptHandle interruptHandle, int32_t* status)
{
    return nullptr;
}

int64_t HAL_WaitForInterrupt(HAL_InterruptHandle interruptHandle,
                             double timeout, HAL_Bool ignorePrevious,
                             int32_t* status)
{
    return 0;
}

void HAL_EnableInterrupts(HAL_InterruptHandle interruptHandle, int32_t* status)
{

}

void HAL_DisableInterrupts(HAL_InterruptHandle interruptHandle,
                           int32_t* status)
{

}

int64_t HAL_ReadInterruptRisingTimestamp(HAL_InterruptHandle interruptHandle,
                                         int32_t* status)
{
    return 0;
}

int64_t HAL_ReadInterruptFallingTimestamp(HAL_InterruptHandle interruptHandle,
                                          int32_t* status)
{
    return 0;
}

void HAL_RequestInterrupts(HAL_InterruptHandle interruptHandle,
                           HAL_Handle digitalSourceHandle,
                           HAL_AnalogTriggerType analogTriggerType,
                           int32_t* status)
{
}

void HAL_AttachInterruptHandler(HAL_InterruptHandle interruptHandle,
                                HAL_InterruptHandlerFunction handler,
                                void* param, int32_t* status)
{

}

void HAL_AttachInterruptHandlerThreaded(HAL_InterruptHandle interruptHandle,
                                        HAL_InterruptHandlerFunction handler,
                                        void* param, int32_t* status)
{

}

void HAL_SetInterruptUpSourceEdge(HAL_InterruptHandle interruptHandle,
                                  HAL_Bool risingEdge, HAL_Bool fallingEdge,
                                  int32_t* status)
{
}
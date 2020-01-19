#include <hal/Notifier.h>

HAL_NotifierHandle HAL_InitializeNotifier(int32_t* status)
{
    return 0;
}

void HAL_SetNotifierName(HAL_NotifierHandle notifierHandle, const char* name,
                         int32_t* status)
{

}

void HAL_StopNotifier(HAL_NotifierHandle notifierHandle, int32_t* status)
{

}

void HAL_CleanNotifier(HAL_NotifierHandle notifierHandle, int32_t* status)
{

}

void HAL_UpdateNotifierAlarm(HAL_NotifierHandle notifierHandle,
                             uint64_t triggerTime, int32_t* status)
{

}

void HAL_CancelNotifierAlarm(HAL_NotifierHandle notifierHandle,
                             int32_t* status)
{

}

uint64_t HAL_WaitForNotifierAlarm(HAL_NotifierHandle notifierHandle,
                                  int32_t* status)
{
    return 0;
}                                  

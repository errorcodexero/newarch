#include <hal/DriverStation.h>

int32_t HAL_SendError(HAL_Bool isError, int32_t errorCode, HAL_Bool isLVCode,
                      const char* details, const char* location,
                      const char* callStack, HAL_Bool printMsg)
{
    return 0;
}

int32_t HAL_GetControlWord(HAL_ControlWord* controlWord)
{
    return 0;
}

HAL_AllianceStationID HAL_GetAllianceStation(int32_t* status)
{
    return HAL_AllianceStationID_kRed1;
}

int32_t HAL_GetJoystickAxes(int32_t joystickNum, HAL_JoystickAxes* axes)
{
    return 0;
}

int32_t HAL_GetJoystickPOVs(int32_t joystickNum, HAL_JoystickPOVs* povs)
{
    return 0;
}

int32_t HAL_GetJoystickButtons(int32_t joystickNum, HAL_JoystickButtons* buttons)
{
    return 0;
}

int32_t HAL_GetJoystickDescriptor(int32_t joystickNum, HAL_JoystickDescriptor* desc)
{
    return 0;
}

HAL_Bool HAL_GetJoystickIsXbox(int32_t joystickNum)
{
    return 0;
}

int32_t HAL_GetJoystickType(int32_t joystickNum)
{
    return 0;
}

char* HAL_GetJoystickName(int32_t joystickNum)
{
    static char name[128] = "BOB";
    return name;
}
void HAL_FreeJoystickName(char* name)
{
}  
int32_t HAL_GetJoystickAxisType(int32_t joystickNum, int32_t axis)
{
    return 0;
}  
int32_t HAL_SetJoystickOutputs(int32_t joystickNum, int64_t outputs,
                               int32_t leftRumble, int32_t rightRumble)
{
    return 0;
}                               
double HAL_GetMatchTime(int32_t* status)
{
    return 0.0;
}

int32_t HAL_GetMatchInfo(HAL_MatchInfo* info)
{
    return 0;
}

void HAL_ReleaseDSMutex(void)
{
}

void HAL_WaitForCachedControlData(void)
{
}

HAL_Bool HAL_WaitForCachedControlDataTimeout(double timeout)
{
    return 0;
}

HAL_Bool HAL_IsNewControlData(void)
{
    return 0;
}

void HAL_WaitForDSData(void)
{
}

HAL_Bool HAL_WaitForDSDataTimeout(double timeout)
{
    return 0;
}

void HAL_InitializeDriverStation(void)
{
}

void HAL_ObserveUserProgramStarting(void)
{
}

void HAL_ObserveUserProgramDisabled(void)
{
}

void HAL_ObserveUserProgramAutonomous(void)
{
}

void HAL_ObserveUserProgramTeleop(void)
{
}

void HAL_ObserveUserProgramTest(void)
{
}

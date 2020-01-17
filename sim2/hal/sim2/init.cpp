#include <hal/HALBase.h>
#include <engine/SimulatorEngine.h>

HAL_Bool HAL_Initialize(int32_t timeout, int32_t mode)
{
    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
    auto code = engine.start();
    if (code != xero::sim2::SimulatorEngine::ErrorCode::NoError)
        return false;

    return true ;
}

uint64_t HAL_GetFPGATime(int32_t *status)
{
    return 0;
}

int32_t HAL_GetFPGAVersion(int32_t *status)
{
    return 0;
}

int64_t HAL_GetFPGARevision(int32_t *status)
{
    return 0;
}

HAL_Bool HAL_GetFPGAButton(int32_t *status)
{
    return 0;
}

HAL_Bool HAL_GetSystemActive(int32_t *status)
{
    return 0;
}

HAL_Bool HAL_GetBrownedOut(int32_t *status)
{
    return 0;
}
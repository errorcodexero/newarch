#include <hal/Relay.h>
#include <hal/Ports.h>

int32_t HAL_GetNumRelayHeaders()
{
    return 0;
}

HAL_RelayHandle HAL_InitializeRelayPort(HAL_PortHandle portHandle, HAL_Bool fwd,
                                        int32_t* status)
{
    return 0;
}            
                            
void HAL_FreeRelayPort(HAL_RelayHandle relayPortHandle)
{
}
HAL_Bool HAL_CheckRelayChannel(int32_t channel)
{
    return 0;
}
void HAL_SetRelay(HAL_RelayHandle relayPortHandle, HAL_Bool on,
                  int32_t* status)
{

}
HAL_Bool HAL_GetRelay(HAL_RelayHandle relayPortHandle, int32_t* status)
{
    return 1;
}
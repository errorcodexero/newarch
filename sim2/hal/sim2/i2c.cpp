#include <hal/Ports.h>
#include <hal/I2C.h>

void HAL_InitializeI2C(HAL_I2CPort port, int32_t* status)
{
}

int32_t HAL_TransactionI2C(HAL_I2CPort port, int32_t deviceAddress,
                           const uint8_t* dataToSend, int32_t sendSize,
                           uint8_t* dataReceived, int32_t receiveSize)
{
    return 0;
}
int32_t HAL_WriteI2C(HAL_I2CPort port, int32_t deviceAddress,
                     const uint8_t* dataToSend, int32_t sendSize)
{
    return 0;
}
int32_t HAL_ReadI2C(HAL_I2CPort port, int32_t deviceAddress, uint8_t* buffer,
                    int32_t count)
{
    return 0;
}
void HAL_CloseI2C(HAL_I2CPort port)
{

}

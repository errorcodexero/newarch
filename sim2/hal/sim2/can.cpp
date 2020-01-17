#include <hal/CAN.h>
#include <hal/Ports.h>

void HAL_CAN_SendMessage(uint32_t messageID, const uint8_t* data,
                         uint8_t dataSize, int32_t periodMs, int32_t* status)
{

}

void HAL_CAN_ReceiveMessage(uint32_t* messageID, uint32_t messageIDMask,
                            uint8_t* data, uint8_t* dataSize,
                            uint32_t* timeStamp, int32_t* status)
{

}

void HAL_CAN_OpenStreamSession(uint32_t* sessionHandle, uint32_t messageID,
                               uint32_t messageIDMask, uint32_t maxMessages,
                               int32_t* status)
{

}

void HAL_CAN_CloseStreamSession(uint32_t sessionHandle)
{

}

void HAL_CAN_ReadStreamSession(uint32_t sessionHandle,
                               struct HAL_CANStreamMessage* messages,
                               uint32_t messagesToRead, uint32_t* messagesRead,
                               int32_t* status)
{

}   

void HAL_CAN_GetCANStatus(float* percentBusUtilization, uint32_t* busOffCount,
                          uint32_t* txFullCount, uint32_t* receiveErrorCount,
                          uint32_t* transmitErrorCount, int32_t* status)
{

}

#pragma once

#include "ErrorBase.h"
#include <cstdint>
#include <cstring>

namespace frc
{
    class I2C
    {
    public:
        enum Port
        {
            Port_0 = 0,
            Port_1 = 1,
            kOnboard = 1,
        } ;

    public:
        I2C(Port port, int deviceAddr);
        virtual ~I2C();

        bool Transaction(uint8_t *dataToSend, int sendSize, uint8_t *dataReceived, int receiveSize);
        bool AddressOnly();
        bool Write(int registerAddress, uint8_t data);
        bool WriteBulk(uint8_t *data, int count);
        bool Read(int registerAddress, int count, uint8_t *data);
        bool ReadOnly(int count, uint8_t *buffer);
        bool VerifySensor(int registerAddress, int count, const uint8_t *expected);


    private:
        int m_i2c_handle;
    };
}


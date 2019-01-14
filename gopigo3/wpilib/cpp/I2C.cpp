#include <frc/I2C.h>
#include <wiringPiI2C.h>
#include <string>
#include <stdexcept>
#include <unistd.h>

namespace frc
{
    I2C::I2C(Port port, int deviceAddr)
    {
        if (port != Port::Port_0)
        {
            std::runtime_error err("invalid port number to I2c constructor, only Port_0 is supported");
            throw err;
        }

        m_i2c_handle = wiringPiI2CSetup(deviceAddr);
        if (m_i2c_handle == -1)
        {
            std::string msg;
            msg = "cannot open I2C device at address ";
            msg += std::to_string(deviceAddr);
            std::runtime_error err(msg.c_str());
            throw err;
        }
    }

    I2C::~I2C()
    {
        ::close(m_i2c_handle);
    }

    bool I2C::Transaction(uint8_t *dataToSend, int sendSize, uint8_t *dataReceived, int receiveSize)
    {
        std::runtime_error err("Transaction method not supported");
        throw err;
    }

    bool I2C::AddressOnly()
    {
        std::runtime_error err("Address Only method not supported");
        throw err;
    }

    bool I2C::Write(int registerAddress, uint8_t data)
    {
        if (!wiringPiI2CWriteReg8(m_i2c_handle, registerAddress, data))
            return false;
        return true;
    }
    
    bool I2C::WriteBulk(uint8_t *data, int count)
    {
        std::runtime_error err("WriteBulk method not supported");
        throw err;
    }

    bool I2C::Read(int registerAddress, int count, uint8_t *data)
    {
        while (count-- > 0)
        {
            int value = wiringPiI2CReadReg8(m_i2c_handle, registerAddress);
            if (value == -1)
                return false;

            *data++ = static_cast<uint8_t>(value);
        }
        return true;
    }

    bool I2C::ReadOnly(int count, uint8_t *buffer)
    {
        while (count-- > 0)
        {
            int value = wiringPiI2CRead(m_i2c_handle);
            if (value == -1)
                return false;

            *buffer++ = static_cast<uint8_t>(value);
        }

        return true;
    }

    bool I2C::VerifySensor(int registerAddress, int count, const uint8_t *expected)
    {
        std::runtime_error err("VerifySensor method not supported");
        throw err;
    }
}

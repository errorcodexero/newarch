#include "GoPiGo3SPI.h"
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>

namespace gopigo3
{
    GoPiGo3SPI::GoPiGo3SPI(const char *devname_p, int bufsize)
    {
        m_spi_handle = ::open(devname_p, O_RDWR);
        if (m_spi_handle < 0)
        {
            std::string msg("cannot open SPI device '");
            msg += devname_p;
            msg += "' in read/write mode";
            std::runtime_error err(msg.c_str());
            throw err;
        }

        m_out_buf.resize(bufsize);
        m_in_buf.resize(bufsize);

        memset(&m_xfer, 0, sizeof(m_xfer));
        m_xfer.cs_change = 0;
        m_xfer.delay_usecs = 0;
        m_xfer.speed_hz = DEFAULT_SPI_SPEED;
        m_xfer.bits_per_word = 8;
    }

    GoPiGo3SPI::~GoPiGo3SPI()
    {
        ::close(m_spi_handle);
    }

    void GoPiGo3SPI::transfer(size_t size)
    {
        int err;

        m_xfer.len = size;
        m_xfer.tx_buf = (unsigned long)&m_out_buf[0];
        m_xfer.rx_buf = (unsigned long)&m_in_buf[0];

        err = ::ioctl(m_spi_handle, SPI_IOC_MESSAGE(1), &m_xfer);
        if (err < 0)
        {
            std::string msg = "ioctl SPI_IOC_MESSAGE(1) failed - error code ";
            msg += std::to_string(err);
            std::runtime_error err(msg.c_str());
            throw err;
        }
    }

    uint8_t GoPiGo3SPI::read8(uint8_t msg_type)
    {
        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;

        transfer(5);

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }

        return m_in_buf[4];
    }

    uint16_t GoPiGo3SPI::read16(uint8_t msg_type)
    {
        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;

        transfer(6);

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }

        return static_cast<uint16_t>((m_in_buf[4] << 8) | m_in_buf[5]);
    }

    uint32_t GoPiGo3SPI::read32(uint8_t msg_type)
    {
        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;

        transfer(8);

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }

        return static_cast<uint32_t>((m_in_buf[4] << 24) | (m_in_buf[5] << 16) | (m_in_buf[6] << 8) | m_in_buf[7]);
    }

    std::string GoPiGo3SPI::readString(uint8_t msg_type, uint8_t len)
    {
        if (len + 4u > m_in_buf.size())
        {
            std::runtime_error err("requested SPI readString with length too long");
            throw err;
        }

        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;

        transfer(len + 4);

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }

        std::string result;
        for (size_t i = 0; i < len; i++)
        {
            if (m_in_buf[i + 4] == 0)
                break;

            result += (char)m_in_buf[i + 4];
        }

        return result;
    }


    void GoPiGo3SPI::sendMessage(uint8_t msg_type, std::vector<uint8_t> &packet) {
        if (packet.size() + 2 > m_out_buf.size())
        {
            std::runtime_error err("packet size too larger for transfer via SPI");
            throw err;
        }

        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;
        for (size_t i = 0; i < packet.size(); i++) {
            m_out_buf[i + 2] = packet[i];
        }

        transfer(packet.size() + 2);

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }
    }

    void GoPiGo3SPI::sendMessage(uint8_t msg_type, std::vector<uint8_t> &packet, std::vector<uint8_t> &result)
    {
        if (packet.size() + 2 > m_out_buf.size())
        {
            std::runtime_error err("packet size too larger for transfer via SPI");
            throw err;
        }

        m_out_buf[0] = m_address;
        m_out_buf[1] = msg_type;
        for (size_t i = 0; i < packet.size(); i++) {
            m_out_buf[i + 2] = packet[i];
        }

        transfer(result.size());

        if (m_in_buf[3] != SPI_VALID_RETURN_VALUE)
        {
            std::runtime_error err("SPI device returned an valid response");
            throw err;
        }

        for (size_t i = 0; i < result.size(); i++)
            result[i] = m_in_buf[i];
    }
}


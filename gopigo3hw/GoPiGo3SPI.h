#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>
#include <linux/spi/spidev.h>

namespace gopigo3
{
	class GoPiGo3SPI
	{
	private:
		const int DEFAULT_SPI_SPEED = 500000;
		const uint8_t SPI_VALID_RETURN_VALUE = 0xA5;

	public:
		GoPiGo3SPI(const char *devname_p, int bufsize);
		~GoPiGo3SPI();

		void setAddress(uint8_t addr) {
			m_address = addr;
		}

		uint8_t getAddress() const {
			return m_address;
		}

		uint8_t read8(uint8_t msg_type);
		uint16_t read16(uint8_t msg_type);
		uint32_t read32(uint8_t msg_type);
		std::string readString(uint8_t msg_type, uint8_t len = 20);

		void sendMessage(uint8_t msg_type, std::vector<uint8_t> &packet);

		void sendMessage(uint8_t msg_type, std::vector<uint8_t> &packet, std::vector<uint8_t> &result);

	private:
		void transfer(size_t size);

	private:
		uint8_t m_address;
		int m_spi_handle;
		std::vector<uint8_t> m_out_buf;
		std::vector<uint8_t> m_in_buf;
		spi_ioc_transfer m_xfer;
	};
}


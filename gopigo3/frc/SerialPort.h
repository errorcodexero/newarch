#pragma once

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <vector>
#include <cstdint>
#include <string.h>
#include <iostream>

namespace frc
{
	class SerialPort
	{
	public:
		enum class Port
		{
			Port_0 = 0,
			Port_1 = 1,
			kMXP,
			kOnboard,
			kUSB
		};

	public:
		SerialPort(Port port);
		SerialPort(int baud, Port port);

		virtual ~SerialPort()
		{
			if (m_handle != -1)
				::close(m_handle);
		}

		void Flush()
		{
		}

		int GetBytesReceived();

		int Read(char *buffer, int count);
		int Write(char *buffer, int count);

		void SetReadBufferSize(int size)
		{
			m_buffer.resize(size);
			m_used = 0;
		}

		void SetTimeout(double timeout)
		{
			m_timeout = timeout;
		}

		void EnableTermination(char terminator = '\n')
		{
			m_term_mode = true;
			m_term = terminator;
		}

		void Reset();

	private:
		void GetData();

	private:

		const char *PortToName(Port p)
		{
			const char *name_p = nullptr;

			switch (m_port)
			{
			case Port::Port_0:
				name_p = "/dev/ttyACM0";
				break;

			default:
				name_p = nullptr;
				break;
			}

			return name_p;
		}

		void open();



	private:
		Port m_port;
		int m_baud;
		int m_handle;
		bool m_term_mode;
		char m_term;
		size_t m_used;
		double m_timeout;
		std::vector<uint8_t> m_buffer;
	};
}


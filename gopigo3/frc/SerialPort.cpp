#include "SerialPort.h"
#include <algorithm>
#include <iostream>

namespace frc
{
	SerialPort::SerialPort(Port port)
	{
		m_port = port;
		m_handle = -1;
		SetReadBufferSize(256);
	}

	SerialPort::SerialPort(int baud, Port port)
	{
		m_port = port;
		m_baud = baud;
		m_handle = -1;
		SetReadBufferSize(256);

		open();
	}

	void SerialPort::open()
	{
		const char *name_p = PortToName(m_port);

		m_handle = ::open(name_p, O_RDWR | O_NOCTTY | O_NDELAY, 0);
		if (m_handle != -1)
		{
			::fcntl(m_handle, F_SETFL, FNDELAY);

			struct termios options;
			if (tcgetattr(m_handle, &options) == -1)
			{
				std::cout << "SerialPort: tcgetattr failed, errno is " << errno << std::endl;
				::close(m_handle);
				m_handle = -1;
				return;
			}
			cfsetispeed(&options, B57600);
			cfsetospeed(&options, B57600);

			options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
			options.c_oflag = 0;
			options.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG | IEXTEN);

			options.c_cflag |= (CLOCAL | CREAD);

			// Eight bits, no parity
			options.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
			options.c_cflag |= CS8;
			options.c_cflag |= CRTSCTS;

			if (tcsetattr(m_handle, TCSANOW, &options) == -1)
			{
				std::cout << "SerialPort: tcsetattr failed, errno is " << errno << std::endl;
				::close(m_handle);
				m_handle = -1;
				return;
			}
		}
	}

	void SerialPort::Reset()
	{
		::close(m_handle);
		open();
	}

	int SerialPort::Write(char *buffer, int count)
	{
		int written = 0;
		while (count > 0)
		{
			int actual = ::write(m_handle, buffer, count);
			if (actual != count)
			count -= actual;
			written += actual;

			buffer += actual;
		}

		return written;
	}

	int SerialPort::GetBytesReceived()
	{
		int ret = m_used;
		int bytes = 0;

		if (::ioctl(m_handle, FIONREAD, &bytes) != -1)
		{
			ret += bytes;
		}

		return ret;
	}

	int SerialPort::Read(char *buffer, int count)
	{
		int bytes;
		int ret = 0;
		bool term = false;

#ifdef _PRINT_ME
		std::cout << "Read, ask for " << count << " bytes" << std::endl;
#endif
		while (count > 0 && !term)
		{
			if (m_used != m_buffer.size())
			{
				if (::ioctl(m_handle, FIONREAD, &bytes) != -1)
				{
					size_t toread = m_buffer.size() - m_used;

					if (toread > static_cast<size_t>(bytes))
						toread = bytes;
					int actual = ::read(m_handle, &m_buffer[m_used], toread);
					m_used += actual;

#ifdef _PRINT_ME
					std::cout << "Physical Read: before " << m_used - actual << ", after " << m_used << std::endl;
					for (size_t i = 0; i < m_used; i++)
						std::cout << std::hex << (int)m_buffer[i] << ' ';
					std::cout << std::dec << std::endl;
#endif

				}
			}

			size_t index = 0;
			while (index < m_used && count > 0)
			{
				*buffer++ = m_buffer[index];
				count--;
				index++;
				ret++;
				if (m_term_mode && m_buffer[index - 1] == m_term)
				{
					term = true;
					break;
				}
			}
#ifdef _PRINT_ME
			std::cout << "Returning " << ret << " bytes from buffer" << std::endl;
#endif

			if (index != 0)
			{
				//
				// remove the first index characters from the buffer
				//
				if (index != m_used)
					memmove(&m_buffer[0], &m_buffer[index], m_buffer.size() - index);

				m_used -= index;
#ifdef _PRINT_ME
				std::cout << "Updated Buffer: m_used " << m_used << std::endl;
				for (size_t i = 0; i < m_used; i++)
					std::cout << std::hex << (int)m_buffer[i] << ' ';
				std::cout << std::dec << std::endl;
#endif
			}
		}

		return ret;
	}
}

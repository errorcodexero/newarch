#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <unistd.h>

class TcpSocket
{
public:
	TcpSocket()
	{
		m_socket = -1;
	}

	virtual ~TcpSocket()
	{
		close();
	}

	virtual void close()
	{
		if (m_socket != -1)
			::close(m_socket);
	}

	virtual bool write(std::vector<uint8_t> &data)
	{
		return write(data, 0, data.size());
	}

	virtual bool write(std::vector<uint8_t> &data, size_t start, size_t count)
	{
		while (count > 0)
		{
			int n = ::write(m_socket, &data[start], count);
			if (n < 0)
				return false;

			start += n;
			count -= n;
		}

		return true;
	}

	virtual bool write(const void *data_p, size_t count)
	{
		const uint8_t *ptr_p = static_cast<const uint8_t *>(data_p);
		while (count > 0)	
		{
			int n = ::write(m_socket, ptr_p, count);
			if (n < 0)
				return false;

			count -= n;
			ptr_p += n;
		}

		return true;
	}

	virtual bool read(std::vector<uint8_t> &data)
	{
		return false;
	}

protected:
	void setSocket(int socket)
	{
		m_socket = socket;
	}

private:
	int m_socket;
};


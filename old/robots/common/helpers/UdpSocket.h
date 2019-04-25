#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace bwgnet
{
	class UdpSocket
	{
	public:
		UdpSocket()
		{
			m_socket = -1;
		}

		virtual ~UdpSocket()
		{
			if (m_socket != -1)
				destroySocket();
		}

		bool isOpen()
		{
			return m_socket != -1;
		}

	protected:
		int getSocket()
		{
			return m_socket;
		}

		bool createSocket()
		{
			m_socket = socket(AF_INET, SOCK_DGRAM, 0);
			if (m_socket < 0)
				return false;


			return true;
		}

		bool destroySocket()
		{
			if (m_socket != -1)
			{
				::close(m_socket);
				m_socket = -1;
			}

			return true;
		}

		bool setBroadcast()
		{
			int broad = 1;

			if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &broad, sizeof(broad)))
			{
				int err = errno;
				std::cout << "cannot setsockopt to broadcast " << err << std::endl;
				return false;
			}

			return true;
		}

	private:
		int m_socket;
	};
}

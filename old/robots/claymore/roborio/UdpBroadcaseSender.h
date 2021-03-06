#pragma once

#include "UdpSocket.h"
#include <string.h>

namespace bwgnet
{
	class UdpBroadcastSender : public UdpSocket
	{
	public:
		UdpBroadcastSender()
		{
		}

		virtual ~UdpBroadcastSender()
		{
		}

		bool open(const std::string &addr, uint16_t port)
		{
			if (isOpen())
				return false;

			if (!createSocket())
				return false;

			if (!setBroadcast())
			{
				destroySocket();
				return false;
			}

			memset(&m_saddr, 0, sizeof(m_saddr));
			m_saddr.sin_family = AF_INET;
			if (addr.length() == 0)
			{
				m_saddr.sin_addr.s_addr = INADDR_BROADCAST;
			}
			else
			{
				if (inet_aton(addr.c_str(), &m_saddr.sin_addr) == 0)
				{
					destroySocket();
					return false;
				}
			}
			m_saddr.sin_port = htons(port);
			return true;
		}

		bool open(uint16_t port)
		{
			std::string empty;
			return open(empty, port);
		}

		bool close()
		{
			return destroySocket();
		}

		bool send(const std::string &msg)
		{
			ssize_t ret = ::sendto(getSocket(), msg.c_str(), msg.length(), 0, (struct sockaddr *)&m_saddr, sizeof(m_saddr));
			if (ret == -1 || static_cast<size_t>(ret) != msg.length())
			{
				int err = errno;
				std::cout << "cannot send broadcast packet - error " << err << std::endl;
				return false;
			}

			return true;
		}

	private:
		struct sockaddr_in m_saddr;
	};
}


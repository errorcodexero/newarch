#pragma once

#include "UdpSocket.h"
#include <vector>
#include <string.h>

namespace bwgnet
{
	class UdpBroadcastReceiver : public UdpSocket
	{
	public:
		UdpBroadcastReceiver()
		{
			m_buffer.resize(256);
		}

		UdpBroadcastReceiver(size_t size)
		{
			m_buffer.resize(size);
		}

		virtual ~UdpBroadcastReceiver()
		{
		}

		bool open(const std::string &addr, uint16_t port)
		{
			if (!createSocket())
				return false;

			if (!setBroadcast())
			{
				destroySocket();
				return false;
			}

			if (!bind(addr, port))
			{
				destroySocket();
				return false;
			}

			return true;
		}

		bool open(uint16_t port)
		{
			std::string empty;
			return open(empty, port);
		}

		bool receive(std::string &msg)
		{
			struct sockaddr_in srcaddr;
			size_t len = sizeof(srcaddr);
			int flags = 0;
			ssize_t count;

			count = recvfrom(getSocket(), &m_buffer[0], m_buffer.size(), flags, (sockaddr *)&srcaddr, &len);
			if (count == -1)
				return false;

			msg = std::string(&m_buffer[0], count);
			return true;
		}

	protected:
		bool bind(const std::string &addr, uint16_t port)
		{
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

			if (::bind(getSocket(), (struct sockaddr *)&m_saddr, sizeof(m_saddr)) == -1)
			{
				destroySocket();
				return false;
			}

			return true;
		}

	private:
		struct sockaddr_in m_saddr;
		std::vector<char> m_buffer;
	};
}

#pragma once

#include "TcpSocket.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>

namespace xeromisc
{
	class TcpServerSocket : public TcpSocket
	{
	public:
		TcpServerSocket()
		{
		}

		virtual ~TcpServerSocket()
		{
		}

		bool create(uint16_t port)
		{
			struct sockaddr_in addr;

			int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0)
				return false;

			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = INADDR_ANY;

			if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
			{
				::close(sockfd);
				return false;
			}

			if (listen(sockfd, 5) == -1)
			{
				::close(sockfd);
				return false;
			}

			size_t addrlen = sizeof(m_client_addr);
			int newsock = ::accept(sockfd, (struct sockaddr *)&m_client_addr, &addrlen);
			if (newsock < 0)
			{
				::close(sockfd);
				return false;
			}

			::close(sockfd);
			setSocket(newsock);
			return true;
		}

	private:
		struct sockaddr_in m_client_addr;
	};
}

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
	class TcpClientSocket : public TcpSocket
	{
	public:
		TcpClientSocket()
		{
		}

		virtual ~TcpClientSocket()
		{
		}

		bool open(const std::string &host, uint16_t port)
		{
			int sockfd;
			struct sockaddr_in saddr;

			memset(&saddr, 0, sizeof(saddr));

			if (!inet_aton(host.c_str(), &saddr.sin_addr))
				return false;

			saddr.sin_family = AF_INET;
			saddr.sin_port = htons(port);

			sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0)
				return false;

			if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
			{
				::close(sockfd);
				return false;
			}

			setSocket(sockfd);
			return true;
		}
	};
}


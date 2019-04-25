#pragma once
#include "sync.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cassert>

class udpSync :public sync {
private:
	struct sockaddr_in myaddr;
	int fd; //socket handle
	unsigned int mPort;
public:
	udpSync(unsigned int port);
	void getMessage(const std::vector<std::string>&);
	void sendMessage();
	unsigned int getPort() const;
};

#pragma once

#include "data_logger_destination.h"
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>


//Set up a client class

template <class DATATYPE>
class DataloggerDestinationServer : public DataloggerDestination<DATATYPE> {
private:
	char *message;
	int fd;
	struct sockaddr_in clientAddr, serverAddr;
	char* createMsg(const std::string&, DATATYPE& data);
	void endLoop(const vector<string>& columns, const vector<DATATYPE>& data);
public:
	DataloggerDestinationServer(const char *remoteAddr, unsigned int port);
};

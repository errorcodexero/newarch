#ifndef _NETWORK_H_
#define _NETWORK_H_

void *getInAddr(struct sockaddr *sa);

class Client {
public:
	Client(const char* _szIP, const char* _szPort, int _iSockType, char _szDelimiter);
	virtual ~Client();

	bool bRecv();	//TCP
	bool bSendTo(const char* _szBuf); //UDP

	char* szGetData();

	//bool bNewIP(const char* _szIP);
private:

	int iSockfd;
	int iSockType;

	const char* szPORT;
	const char* szIP;
	char szBuf[512];
	char szDelimiter;

	struct addrinfo *aiP;

};
/*
class Server {
public:

	Server(const char* _szPort, int _iSockType, int _iBacklog, unsigned int _iMaxClients, bool _bIsPersistant);
	virtual ~Server();

	char* szGetData();

	bool bBroadcast(const char* _szBuf); //Only use if your using TCP sockets
	bool bListen(); //Only use if your using UDP sockets

private:

	int iSockfd;
	int iNewfd;
	int iYes = 1;
	int iRV;
	int iSockType;
	int iBacklog;
	int iNumBytes;

	bool bIsPersistant;

	unsigned int iMaxClients;

	char szS[INET6_ADDRSTRLEN];
	char* szBuf;

	const char* szPORT;

	struct addrinfo aiHints, *aiServInfo, *aiP;

	socklen_t slAddrLen, slSinSize;

	struct sigaction sa;

	struct sockaddr_storage saCliAddr;

};
*/
#endif /* _NETWORK_H_ */
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdarg.h>

#include <vector>
#include <cassert>

//#include "string_utils.h"

using namespace std;

void *getInAddr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int sendAll(int _iS, char* _szBuf, int* _iLen) {
	int iTotal = 0;
	int iBytesLeft = *_iLen;
	int iN;

	while (iTotal < *_iLen) {
		iN = send(_iS, _szBuf+iTotal, iBytesLeft, 0);
		if (iN == -1 ) { break; }
		iTotal += iN;
		iBytesLeft -=iN;
	}

	*_iLen = iTotal;

	assert(0);//next line invokes undefined behavior, so if we ever get here then abort!
	return iN==-1?-1:0;
}
/*
void sigChildHandler(int s)
{
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}*/
Client::Client(const char* _szIP, const char* _szPort, int _iSockType, char _szDelimiter = '\n') {
	vector <char* > vszIP;
	//int iPort = 0; //Only a temporary, validation value
	//do some validation
	//vszIP = vszParseString(_szIP, '.');
	////cout << vszIP.size() << "\n";
	//if (vszIP.size() != 3)
		//exitWithError("Not a valid IP", 3);

	//if (sscanf(_szPort, "%d", &iPort) < 1)
	//	exitWithError("Port Number contains no integers", 5);
	//if (iPort > 65535 || iPort < 0)
	//	exitWithError("Port number is not valid", 4);

	//Set up necessary local variables
	
	struct addrinfo aiHints, *aiServInfo;
	
	szPORT = _szPort;
	szIP = _szIP;
	iSockType = _iSockType;
	szDelimiter = _szDelimiter;
	szBuf[0] = '\0';
	
	int iRV = 0;
	char szS[INET6_ADDRSTRLEN];

	memset(&aiHints, 0, sizeof aiHints);
	aiHints.ai_family = AF_UNSPEC;
	aiHints.ai_socktype = iSockType;

	if ((iRV = getaddrinfo(szIP, szPORT, &aiHints, &aiServInfo)) != 0) {
		fprintf(stderr, "getAddrInfo: %s\n", gai_strerror(iRV));
		//exit(1);
	}

	for(aiP = aiServInfo; aiP != NULL; aiP = aiP->ai_next) {
	        if ((iSockfd = socket(aiP->ai_family, aiP->ai_socktype,
	        		aiP->ai_protocol)) == -1) {
	            perror("client: socket");
	            continue;
	        }
	        if (iSockType == SOCK_STREAM) {
	        	if (connect(iSockfd, aiP->ai_addr, aiP->ai_addrlen) == -1) {
	        		close(iSockfd);
	            	perror("client: connect");
	            	continue;
	        	}
	        } else if (iSockType == SOCK_DGRAM) {
	        	//Doesn't need a special connect statement
	        } else {
	        	//exitWithError("Not Valid Socket Type, or Socket Type Not Yet Implemented",6);
	        }
	        break;
	}

	if (aiP == NULL) {
	        fprintf(stderr, "client: failed to connect\n");
	        exit(2);
	    }

	inet_ntop(aiP->ai_family, getInAddr((struct sockaddr *)aiP->ai_addr),
	            szS, sizeof szS);
	fcntl(iSockfd, F_SETFL, O_NONBLOCK);
	printf("client: connecting to %s\n", szS);

	freeaddrinfo(aiServInfo);

}

Client::~Client() {
	close(iSockfd);

}

bool Client::bRecv() {
	int iNumBytes = 0;
	memset(szBuf, 0, sizeof szBuf);
	unsigned int i = 0;
	char szTmpBuffer[1];
	if (iSockfd == -1)
		exit(1);
	if(iSockType == SOCK_STREAM) {
		////cout << sizeof szBuf << "\n";
		do	{
			if ((iNumBytes = recv(iSockfd, szTmpBuffer, 1, 0)) == -1 /* && //cout << szTmpBuffer*/) {
				return false;
			} else {
				i++;
				//szBuf[0] = szTruncateByDelimiter(&szBuf[0], szDelimiter);
				if (i > sizeof szBuf - 1){
					break;
				}
				////cout << index;
				szBuf[i] = szTmpBuffer[0];
				////cout << szTmpBuffer << " "<< szBuf[i] << "\n";
				//szTmpBuffer[0] = '\0';
			}

		} while (szTmpBuffer[0] != szDelimiter);
		//cout << szBuf;

		return true;
	} else {
		//cout << "Error: Not Stream Socket\n";
		return false;
	}
	return false;
}

bool Client::bSendTo(const char* _szBuf) {
	if(iSockType == SOCK_DGRAM) {
		int iNumBytes = 0;
		if ((iNumBytes = sendto(iSockfd, _szBuf, strlen(_szBuf), 0, aiP->ai_addr, aiP->ai_addrlen)) == -1) {
		        perror("talker: sendto");
		        exit(1);
		        return false;
		    }
		else
			return true;
	} else {
		//cout << "Error: Not Datagram Socket\n";
		return false;
	}
}

/*bool Client::bNewIP(const char* _szIP) {
#ifdef DEBUG
	//cout << "NOTE: This function clears the buffer\n";
#endif
	int i = sizeof szBuf;
	memset(szBuf, 0, i);
	int x = sizeof szIP;
	if(bIsStringIP(_szIP)) {
		memcpy(szIP, _szIP, x);

	}
	else
		return false;

	return true;
}*/


char* Client::szGetData() {
	return szBuf;
}
/*
Server::Server (const char* _szPort, int _iSockType, int _iBacklog, unsigned int _iMaxClients, bool _bIsPersistant) {

	szPORT = _szPort;
	iSockType = _iSockType;
	iBacklog = _iBacklog;
	iMaxClients = _iMaxClients;
	iNewfd = 0;
	szBuf = '\0';
	iNumBytes = 0;
	slSinSize = 0;
	bIsPersistant = _bIsPersistant;

	memset(&aiHints, 0, sizeof aiHints);
	aiHints.ai_family = AF_UNSPEC;
	aiHints.ai_socktype = iSockType;
	aiHints.ai_flags = AI_PASSIVE;

	if ((iRV = getaddrinfo(NULL, szPORT, &aiHints, &aiServInfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(iRV));
		//exit(1);
	}

	for (aiP = aiServInfo; aiP != NULL; aiP = aiP->ai_next) {

		if ((iSockfd = socket(aiP->ai_family, aiP->ai_socktype, aiP->ai_protocol)) == -1) {
			perror("Server: Socket");
			continue;
		}

		if (setsockopt(iSockfd, SOL_SOCKET, SO_REUSEADDR, &iYes, sizeof(int)) == -1) {
		            perror("setsockopt");
		            exit(1);
		        }

		if (bind(iSockfd, aiP->ai_addr, aiP->ai_addrlen) == -1) {
			close(iSockfd);
			perror("Server: Bind");
			continue;
		}

		break;
	}
	if (aiP == NULL) {
		fprintf(stderr, "Server: failed to bind socket\n");
		//exit(2);
	}

	freeaddrinfo(aiServInfo);

	if (iSockType == SOCK_STREAM) {

		if (listen(iSockfd, iBacklog) == -1) {

			perror("Listen");
			//exit(1);
		}

		sa.sa_handler = sigChildHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1) {

			perror("sigaction");
			//exit(1);
		}

	} else if (iSockType == SOCK_DGRAM) {
		slAddrLen = sizeof saCliAddr;

	} else {

		exitWithError("Socket type not available", 3);
	}

}

Server::~Server() {
	szBuf = (char *) "";
	if (iSockfd == -1 && iNewfd == -1){
#ifdef DEBUG
		//cout << "Socket Closed\n";
#endif
	}
	else{
		if(shutdown(iNewfd, 2) == -1 || shutdown(iSockfd, 2) == -1) {
			if (iSockfd == -1) {
				exitWithError("New socket Failed to close", 1);
			}
			if (iNewfd == -1) {
				exitWithError("Socket Failed to close", 1);
			}
		}
	}
}

bool Server::bBroadcast(const char* _szBuf) {
	unsigned int i = 0; //for sake of comparison errors
	//szBuf = _szBuf;

	while (i < iMaxClients) {
		//cout << i << "\n";
		slSinSize = sizeof saCliAddr;
		iNewfd = accept(iSockfd, (struct sockaddr *)&saCliAddr, &slSinSize);

		if(iNewfd == -1) {
			return false;
			continue;
		}
		inet_ntop(saCliAddr.ss_family, getInAddr((struct sockaddr *)&saCliAddr), szS, sizeof szS);

		if (!fork()) {
			close(iSockfd);
			int iBufLen = strlen(szBuf);
			if (sendAll(iNewfd, (char *)_szBuf, &iBufLen) == -1)
				return false;
			//close(iNewfd);
			//exit(0);
		}
		close(iNewfd);
		i++;
	}
	return true;
}

bool Server::bListen() {
	if ((iNumBytes = recvfrom(iSockfd, szBuf, sizeof(szBuf-1), 0, (struct sockaddr *)&saCliAddr, &slAddrLen)) == -1) {
		perror("recvform");
		return false;
	}
	szBuf[iNumBytes] = '\0';
	return true;
}

char* Server::szGetData() {
	return szBuf;
}
*/

#ifdef NETWORK_TEST
int main() {
	
}
#endif

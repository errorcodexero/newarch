#include "udp_sync.h"

udpSync::udpSync(unsigned int port) {
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
	}
	/* bind to an arbitrary return address */
	/* because this is the client side, we don't care about the address */
	/* since no application will initiate communication here - it will */
	/* just send responses */
	/* INADDR_ANY is the IP address and 0 is the socket */
	/* htonl converts a long integer (e.g. address) to a network representation */
	/* htons converts a short integer (e.g. port) to a network representation */
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
	}
	mPort = ntohs(myaddr.sin_port);
}


void udpSync::getMessage(const std::vector<std::string>&) {

}

void udpSync::sendMessage() {

}

unsigned int udpSync::getPort() const {
	return mPort;
}



#ifdef UDP_SYNC_TEST
int main() {
	udpSync snc(591);
	int port = snc.getPort();
	assert(port== 591);
}
#endif // UDP_SYNC_TEST

#pragma once

#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <net/if.h>   // Tested with cygwin cross-compilation for Raspberry Pi
#include <string>
#include <vector>


namespace xero {
    namespace misc {

        /// \brief Return vector of IP4 interfaces in string form.  Only active ones.  Exclude loopback.
        std::vector<std::string> get_host_ip_addresses() {
            struct ifaddrs * ifAddrStruct=NULL;
            struct ifaddrs * ifa=NULL;
            void * tmpAddrPtr=NULL;
            std::vector<std::string> result;

            getifaddrs(&ifAddrStruct);

            for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
                if (!ifa->ifa_addr) {
                    continue;
                }

                // Only those running and not loopback
                if ((ifa->ifa_flags & IFF_RUNNING) == 0 ||
                    (ifa->ifa_flags & IFF_LOOPBACK) != 0) {
                    continue;
                }
                
                if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
                    // is a valid IP4 Address
                    tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                    char addressBuffer[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                    //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

                    // Only add IP4 addresses to the return value
                    result.push_back(addressBuffer);
                } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
                    // is a valid IP6 Address
                    tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                    char addressBuffer[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                    //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
                } 
            }
            if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
            return result;
        }

    }
}

#pragma once

#include "UdpSocket.h"
#include <vector>
#include <string.h>
#include <fcntl.h>


/// \file


namespace xero
{
    namespace misc {
        /// \brief This class is a receiver of UDP broadcast packets
        class UdpBroadcastReceiver : public UdpSocket
        {

        public:
            /// \brief create the UDP broadcast receiver
            UdpBroadcastReceiver()
            {
                blocking = true;
            }

            /// \brief destroy the UDP broadcast receiver
            virtual ~UdpBroadcastReceiver()
            {
            }

            /// \brief open the broadcast receiver
            /// \param addr the address to bind the socket to, if empty bind to the INADDR_ANY address
            /// \param port the port to bind the socket to
            /// \returns true if the socket was created, otherwise false
            bool open(const std::string &addr, uint16_t port)
            {
                if (!createSocket())
                    return false;


                if (!bind(addr, port))
                    {
                        destroySocket();
                        return false;
                    }

                return true;
            }

            /// \brief select whether read is blocking or non-blocking
            /// \param set_blocking true if read should be blocking, false otherwise.  Reads are blocking by default.
            void setBlocking(bool set_blocking)
            {
                blocking = set_blocking;
            }


            /// \brief open the broadcast receiver
            /// \param port the port to bind the socket to
            /// \returns true if the socket was created, otherwise false
            bool open(uint16_t port)
            {
                std::string empty;
                return open(empty, port);
            }

            /// \brief receive a package of data from the socket
            /// \param data a vector to store the data, must be sized to the desired receive size
            /// \returns the number of bytes sent, or -1 if there is an error
            int receive(std::vector<uint8_t> &data)
            {
                struct sockaddr_in srcaddr;
                socklen_t len = sizeof(srcaddr);
                int flags = 0;
                ssize_t count;

                if (!blocking) {
                    flags |= MSG_DONTWAIT;
                }

                count = recvfrom(getSocket(), &data[0], data.size(), flags, (sockaddr *)&srcaddr, &len);
                if (count == -1)
                    return -1;

                return count;
            }

        protected:
            /// \brief bind the socket to a given address and port
            /// \param addr the address for the socket, empty means INADDR_ANY
            /// \param port the port for the socket
            /// \returns true if the socket is bound to the address given
            bool bind(const std::string &addr, uint16_t port)
            {
                memset(&m_saddr, 0, sizeof(m_saddr));
                m_saddr.sin_family = AF_INET;
                if (addr.length() == 0)
                    {
                        m_saddr.sin_addr.s_addr = INADDR_ANY;
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
            bool blocking;
	};
    }
}

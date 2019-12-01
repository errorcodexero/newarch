#pragma once

#include "UdpSocket.h"
#include <string.h>
#include <vector>


/// \file

namespace xero
{
    namespace misc {
        /// \brief this class sends messages on a UDP socket
        class UdpSender : public UdpSocket
        {
        public:
            /// \brief create the new UDP socket sender objects
            UdpSender()
            {
            }

            /// \brief destroy the new UDP socket sender object
            virtual ~UdpSender() noexcept
            {
            }

            /// \brief open the UDP socket sender.
            /// \param addr the address to bind the socket to.  If empty, bind to broadcast address
            /// \param port the power to bind the aocket to
            /// \returns true if the socket is opened, otherwise false
            bool open(const std::string &addr, uint16_t port)
            {
                if (isOpen())
                    return false;

                if (!createSocket())
                    return false;

                if (addr.length() == 0)
                {
                    if (!setBroadcast())
                    {
                        destroySocket();
                        return false;
                    }
                }

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
                return true;
            }

            /// \brief open a socket bound to the broadcast address
            /// \param port the port to bind the socket to
            /// \returns true if the socket is opened, otherwise false
            bool open(uint16_t port)
            {
                std::string empty;
                return open(empty, port);
            }

            /// \brief close the socket
            /// \returns true if the socket is closed, otherwise false
            bool close()
            {
                return destroySocket();
            }

            /// \brief send a block of data
            /// \param data the vector of data to send
            /// \returns true if the data is sent sucessfully, otherwise false
            bool send(const std::vector<uint8_t> &data)
            {
                return send(data, 0, data.size());
            }

            /// \brief send a subset of data from within a vector
            /// \param data the vector containing the data to send
            /// \param start the starting position in the vector for the data
            /// \param count the number of bytes to send
            /// \returns true if the data is sent sucessfully, otherwise falsae
            bool send(const std::vector<uint8_t> &data, size_t start, size_t count)
            {
                ssize_t ret = ::sendto(getSocket(), &data[start], count, 0, (struct sockaddr *)&m_saddr, sizeof(m_saddr));
                if (ret == -1 || static_cast<size_t>(ret) != count)
                {
                    int err = errno;
                    std::cout << "cannot send udp packet - error " << err << std::endl;
                    return false;
                }

                return true;
            }

            bool send(const std::string &str) {
                ssize_t ret = ::sendto(getSocket(), str.c_str(), str.length(), 0, (struct sockaddr *)&m_saddr, sizeof(m_saddr));
                if (ret == -1 || static_cast<size_t>(ret) != str.length())
                {
                    int err = errno;
                    std::cout << "cannot send udp packet - error " << err << std::endl;
                    return false;
                }

                return true; 
            }

        private:
            struct sockaddr_in m_saddr;
        };
    }
}

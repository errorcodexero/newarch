#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



/// \file

namespace xero
{
	namespace misc 
	{
		/// \brief This is the base class for a UDP socket.
		class UdpSocket
		{
		public:
			/// \brief create a new socket
			UdpSocket()
			{
				m_socket = -1;
			}

			/// \brief destroy this socket object, closing any open socket
			virtual ~UdpSocket()
			{
				if (m_socket != -1)
					destroySocket();
			}

			/// \brief returns true if this socket is open
			/// \returns true if the socket is open
			bool isOpen()
			{
				return m_socket != -1;
			}

        protected:
                
			/// \brief get the underlying socket from the socket object
			/// \returns the socket associated with the object
			int getSocket()
			{
				return m_socket;
			}

			/// \brief create a new UDP (datagram) socket
            /// \param broadcast if true put the socket in state to receive broadcast packets
			/// \returns true if the socket is created, otherwise false
			bool createSocket(bool broadcast=false)
			{
				m_socket = socket(AF_INET, SOCK_DGRAM, 0);
				if (m_socket < 0)
					return false;

				if (broadcast)
                    setBroadcast();

				return true;
			}

			/// \brief destroy the created socket
			/// \returns true if the socket is destroyed, otherwise false
			bool destroySocket()
			{
				if (m_socket != -1)
				{
					::close(m_socket);
					m_socket = -1;
				}

				return true;
			}

			/// \brief set the sockt object into broadcast mode
			/// \returns true if the socket is set to broadcast, otherwise false
			bool setBroadcast()
			{
				int broad = 1;

				if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &broad, sizeof(broad)))
				{
					int err = errno;
					std::cout << "cannot setsockopt to broadcast " << err << std::endl;
					return false;
				}

				return true;
			}

		private:
			int m_socket;
		};
	}
}

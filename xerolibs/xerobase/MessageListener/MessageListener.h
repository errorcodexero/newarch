#pragma once

#include <list>
#include <Subsystem.h>
#include <UdpReceiver.h>

/// \file

namespace xero {
    namespace base {

        /// \brief A [network] message listener subsystem. Intended to listen to network packets sent from a remote system such as vision running on raspberry.
        class MessageListener : public Subsystem {

        public:
            /// \brief Constructor
            /// \param robot The robot that contains this subsystem.
            /// \param socket_port_number The port number on which the raspberry will broadcast UDP packets with the results.
            MessageListener(Subsystem *parent, int socket_port_number);

            /// \brief Destructor
            virtual ~MessageListener() ;


            /// \brief Check for pending messages. If any found, add them to the list of message.
            virtual void computeState();

            /// \brief Run the subsystem. Nothing to do for this subsystem.
            //virtual void run() ;

            /// \brief This method returns true if the subsystem can execute the given action
            /// \param action the section to test to see if it can be executed
            /// \returns true if the action can be executed, false otherwise
            //virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:

            /// \brief Pointer to UDP baordcast receiver.
            xero::misc::UdpReceiver* m_server_in_p;

            /// \brief Messages received and not processed yet.
            std::list<std::string> messages;
        };
    }
}

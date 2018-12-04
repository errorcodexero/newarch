#pragma once

#include <list>
#include <Subsystem.h>

/// \file

namespace xero {
    namespace base {

        /// \brief A [network] message listener subsystem. Intended to listen to network packets sent from a remote system such as vision running on raspberry.
        class MessageListener : public Subsystem {

        public:
            /// \brief Constructor
            /// \param robot The robot that contains this subsystem.
            MessageListener(Robot& robot);

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
            /// \brief Message received and not processed yet.
            std::list<std::string> messages;
        };
    }
}

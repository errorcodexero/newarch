#include "MessageListener.h"
#include "Robot.h"

//using namespace xero::misc ;

namespace xero {
    namespace base {

        MessageListener::MessageListener(Robot& robot, int socket_port_number) : Subsystem(robot, "MessageListener") {
            m_server_in_p = new xero::misc::UdpBroadcastReceiver();
            m_server_in_p->setBlocking(false);
            if (!m_server_in_p->open(socket_port_number)) {
                std::string err("Message listener could not connect to port");
                std::runtime_error ex(err.c_str());
                throw ex;
            }

        }

        MessageListener::~MessageListener() {
            delete m_server_in_p;
        }

        void MessageListener::computeState() {
            std::vector<uint8_t> data(128);
            int ret = m_server_in_p->receive(data);
            if (ret != -1) {
                std::cout << "MESSAGE RECEIVED  " << ret << "\n";
                exit(0);
            }
            
        }
    }
}

#include <UdpReceiver.h>

#include "MessageListener.h"
#include "Robot.h"

//using namespace xero::misc ;

namespace xero {
    namespace base {

        MessageListener::MessageListener(Robot& robot) : Subsystem(robot, "MessageListener") {
        }

        MessageListener::~MessageListener() {			
        }

        void MessageListener::computeState() {
            
        }
    }
}

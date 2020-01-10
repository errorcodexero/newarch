#include "TestSubsystem.h"
#include "VelocityAction.h"
#include "motorcontrollerids.h"
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
      
        TestSubsystem::TestSubsystem(Subsystem *parent) : MotorEncoderSubsystem(parent, "testsubsystem", MSG_GROUP_TESTSUBSYSTEM) {
            setSmartDashboardName("testsubsystem") ;
        }

        void TestSubsystem::postHWInit() {
            setDefaultAction(std::make_shared<VelocityAction>(*this));
            MotorEncoderSubsystem::postHWInit();
        }

        TestSubsystem::~TestSubsystem() {
        }
    }
}

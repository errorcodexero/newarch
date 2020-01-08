#include "TestSubsystem.h"
#include "VelocityAction.h"
#include "motorcontrollerids.h"
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
      
        TestSubsystem::TestSubsystem(Subsystem *parent) : MotorEncoderSubsystem(parent, "testsubsystem", MSG_GROUP_TESTSUBSYSTEM, true) {
            setSmartDashboardName("testsubsystem") ;

            int ain = parent->getRobot().getSettingsParser().getInteger("hw:testsubsystem:input");
            ainput_ = std::make_shared<frc::AnalogInput>(ain);

            setDefaultAction(VelocityAction(this));
        }

        TestSubsystem::~TestSubsystem() {
        }

        void TestSubsystem::computeState() {
            MotorEncoderSubsystem::computeState();
            avalue_ = ainput_->GetVoltage();
        }
    }
}

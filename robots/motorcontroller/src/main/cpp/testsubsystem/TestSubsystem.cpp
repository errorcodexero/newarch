#include "TestSubsystem.h"
#include "VelocityAction.h"
#include "motorcontrollerids.h"
#include <Robot.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
      
        TestSubsystem::TestSubsystem(Subsystem *parent) : MotorEncoderSubsystem(parent, "testsubsystem", MSG_GROUP_TESTSUBSYSTEM) {
            setSmartDashboardName("testsubsystem") ;
        }

        void TestSubsystem::init(xero::base::LoopType ltype) {
            if (ltype == LoopType::OperatorControl) {
                //setDefaultAction(std::make_shared<SingleMotorPowerAction>(*this, 0.3));
                setAction(std::make_shared<VelocityAction>(*this));
            }
            MotorEncoderSubsystem::init(ltype);
        }

        void TestSubsystem::computeState() {
            frc::SmartDashboard::PutNumber("avel", getSpeedometer().getVelocity());
            MotorEncoderSubsystem::computeState();
        }

        TestSubsystem::~TestSubsystem() {
        }
    }
}

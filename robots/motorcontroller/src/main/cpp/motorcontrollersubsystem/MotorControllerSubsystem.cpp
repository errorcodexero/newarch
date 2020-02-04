#include "MotorControllerSubsystem.h"
#include "motorcontrollerids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
        MotorControllerSubsystem::MotorControllerSubsystem(Robot &robot) : RobotSubsystem(robot, "motorcontroller") {

            addTankDrive() ;

            oi_ = std::make_shared<MotorControllerOISubsystem>(this) ;
            addChild(oi_) ;

            test_ = std::make_shared<TestSubsystem>(this);
            addChild(test_);

            sensor_ = std::make_shared<ColorSensorSubsystem>(this) ;
            addChild(sensor_) ;
        }

        MotorControllerSubsystem::~MotorControllerSubsystem() {
        }

        bool MotorControllerSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}

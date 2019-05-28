#include "gopigo3sub/GoPiGo3Subsystem.h"
#include "oi/GoPiGo3OISubsystem.h"
#include <Robot.h>

using namespace xero::base ;

// hw:tankdrive:leftmotor           1
// hw:tankdrive:rightmotor          2
// hw:tankdrive:leftenc:1           0
// hw:tankdrive:leftenc:2           1
// hw:tankdrive:rightenc:1          2
// hw:tankdrive:rightenc:2          3

namespace xero {
    namespace gopigo3 {
        GoPiGo3Subsystem::GoPiGo3Subsystem(Robot &robot) : Subsystem(robot, "gopigo") { 
            servo_ = std::make_shared<ServoSubsystem>(robot) ;
            addChild(servo_) ;

            led_ = std::make_shared<LEDSubsystem>(robot);
            addChild(led_);

            oi_ = std::make_shared<GoPiGo3OISubsystem>(robot) ;
            addChild(oi_) ;

            std::list<int> left_motors ;
            std::list<int> right_motors ;
            left_motors.push_back(robot.getSettingsParser().getInteger("hw:tankdrive:leftmotor")) ;
            right_motors.push_back(robot.getSettingsParser().getInteger("hw:tankdrive:rightmotor")) ;
            tankdrive_ = std::make_shared<TankDrive>(robot, left_motors, right_motors) ;
            tankdrive_->invertRightMotors() ;

            int l1, l2, r1, r2 ;
            l1 = robot.getSettingsParser().getInteger("hw:tankdrive:leftenc:1") ;
            l2 = robot.getSettingsParser().getInteger("hw:tankdrive:leftenc:2") ;
            r1 = robot.getSettingsParser().getInteger("hw:tankdrive:rightenc:1") ;
            r2 = robot.getSettingsParser().getInteger("hw:tankdrive:rightenc:2") ;                                  
            tankdrive_->setEncoders(l1, l2, r1, r2) ;

            addChild(tankdrive_) ;
        }

        GoPiGo3Subsystem::~GoPiGo3Subsystem() {         
        }
    }
}

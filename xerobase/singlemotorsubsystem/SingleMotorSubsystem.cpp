#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, const std::string &motor) : Subsystem(robot,name) {
           int m = robot.getSettingsParser().getInteger(motor) ;

           motor_ = std::make_shared<TalonSRX>(m);
        }

        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, int m) : Subsystem(robot,name) {
           motor_ = std::make_shared<TalonSRX>(m);
        }        

        SingleMotorSubsystem::~SingleMotorSubsystem(){
        }

        bool SingleMotorSubsystem::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
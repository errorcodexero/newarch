#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, const std::string &motor, bool victor) : Subsystem(robot,name) {
        	int m = robot.getSettingsParser().getInteger(motor) ;

			if (victor)
				vmotor_ = std::make_shared<frc::VictorSP>(m) ;
			else
        		tmotor_ = std::make_shared<TalonSRX>(m);
        }

        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, int m, bool victor) : Subsystem(robot,name) {
			if (victor)
				vmotor_ = std::make_shared<frc::VictorSP>(m) ;
			else
        		tmotor_ = std::make_shared<TalonSRX>(m);
        }        

        SingleMotorSubsystem::~SingleMotorSubsystem(){
        }

        bool SingleMotorSubsystem::canAcceptAction(ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}
#include "LineDetectAction.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
            LineDetectAction::LineDetectAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem) : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem) {
            }

            LineDetectAction::~LineDetectAction() {                
            }

            void LineDetectAction::start() {
                is_done_ = false ;
            }

            void LineDetectAction::run() {
                
                is_done_ = ls_subsystem_.detectedObject() ;

                getTankDrive().getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                getTankDrive().getRobot().getMessageLogger() << "line detect action is " <<(is_done_?" done" :" not done");
                getTankDrive().getRobot().getMessageLogger() << " state is " << ls_subsystem_.getSensorsState() ;                
                getTankDrive().getRobot().getMessageLogger().endMessage() ;

            }

            bool LineDetectAction::isDone() {
                return is_done_ ;
            }

            void LineDetectAction::cancel() {
                is_done_ = true ;
            }

            std::string LineDetectAction::toString() {
                std::string result = "LineDetectAction " ;
                return result ;
            }
    }
}

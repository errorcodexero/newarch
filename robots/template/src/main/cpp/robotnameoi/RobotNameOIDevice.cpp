#include "$$RobotName$$OIDevice.h"
#include "$$RobotName$$OISubsystem.h"
#include "$$RobotName$$.h"
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {

        $$RobotName$$OIDevice::$$RobotName$$OIDevice($$RobotName$$OISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ;
        }

        $$RobotName$$OIDevice::~$$RobotName$$OIDevice() {
        }

        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void $$RobotName$$OIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;

            //
            // TODO - map buttons from the OI
            //

        }

        void $$RobotName$$OIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            //
            // TODO - generate the list of actions based on the buttons pushed and the
            //        OI logic
            //
        }

        //
        // Create static actions we for the OI
        //
        void $$RobotName$$OIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            // TODO - create robot specific actions to be called by the OI
        }
    }
}

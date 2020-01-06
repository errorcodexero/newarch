#include "MotorControllerOIDevice.h"
#include "MotorControllerOISubsystem.h"
#include "MotorController.h"
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {

        MotorControllerOIDevice::MotorControllerOIDevice(MotorControllerOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ;
        }

        MotorControllerOIDevice::~MotorControllerOIDevice() {
        }

        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void MotorControllerOIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            ////%% OI_Device_Button_Mappings

            ////%% End OI_Device_Button_Mappings
        }

        void MotorControllerOIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            //
            // TODO - generate the list of actions based on the buttons pushed and the
            //        OI logic
            //
        }

        //
        // Create static actions we for the OI
        //
        void MotorControllerOIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            // TODO - create robot specific actions to be called by the OI
        }
    }
}

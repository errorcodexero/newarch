#include "RanseurOIDevice.h"
#include "RanseurOISubsystem.h"
#include "Ranseur.h"
#include "ranseurrobotsubsystem/RanseurRobotSubsystem.h"
#include <oi/DriverGamepadRumbleAction.h>
#include "ranseurids.h"
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <TerminateAction.h>
#include <DelayAction.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        RanseurOIDevice::RanseurOIDevice(RanseurOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ; 
        }

        RanseurOIDevice::~RanseurOIDevice() {
        }

        void RanseurOIDevice::generateActions(SequenceAction &seq){

            //get tracking mode (auro, semiauto, or manual)
            getTrackingMode() ;
        }

        std::string RanseurOIDevice::toString(OperationMode mode) {
            std::string ret = "?????" ;

            switch(mode) {
                case OperationMode::Auto:
                    ret = "Auto" ;
                    break ;
                case OperationMode::Invalid:
                    ret = "Invalid" ;
                    break ;
                case OperationMode::Manual:
                    ret = "Manual" ;
                    break ;
                case OperationMode::SemiAuto:
                    ret = "SemiAuto" ;
                    break ;
            }

            return ret ;
        }

        //
        // Read the mode switch.  If it differs from the current mode, call UpdateMode()
        // to update the current mode.
        //
        void RanseurOIDevice::getTrackingMode() {
            OperationMode mode = OperationMode::Auto ;
            int value = getValue(tracking_manual_switch_) ;

            switch(value) {
            case 0:
                mode = OperationMode::Auto ;
                break ;
            case 1:
                mode = OperationMode::SemiAuto ;
                break ;
            case 2:
                mode = OperationMode::Manual ;
                break ;
            }
            if (mode != mode_)
                updateMode(mode) ;
        }

        //
        // Update the mode of the robot.  This should changes the camera mode and assocaited LED as necessary
        //
        void RanseurOIDevice::updateMode(OperationMode newmode) {
            std::string newmodestr = toString(newmode) ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: updating mode" ;
            log << ", old mode " << toString(mode_) ;
            log << ", new mode " << newmodestr ; ;
            log.endMessage() ;

            mode_ = newmode ;
            frc::SmartDashboard::PutString("RobotMode", newmodestr) ;
        }
        
        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void RanseurOIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;
            int button ;
            SettingsParser &settings = getSubsystem().getRobot().getSettingsParser() ;
  
            button = settings.getInteger("oi:tracking_viewing_nothing_switch") ;
            tracking_manual_switch_ = mapAxisSwitch(button, 3) ;
        }

        //
        // Create static actions we for the OI
        //
        void RanseurOIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;      
        }
    }
}

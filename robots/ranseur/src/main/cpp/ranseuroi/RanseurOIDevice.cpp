#include "RanseurOIDevice.h"
#include "RanseurOISubsystem.h"
#include "Ranseur.h"
#include "ranseurrobotsubsystem/RanseurRobotSubsystem.h"
#include "ranseurids.h"

#include <oi/DriverGamepadRumbleAction.h>
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <actions/TerminateAction.h>
#include <actions/DelayAction.h>
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
            initialize() ;
        }

        RanseurOIDevice::~RanseurOIDevice() {
        }

        void RanseurOIDevice::initialize() {
            //
            // Get button numbers
            //
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:collect") ;
            size_t dump_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:dump") ;
            size_t turtle_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:turtle") ;
            size_t eject_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:eject") ;
            size_t spare1_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:spare1") ;
            size_t spare2_b = getSubsystem().getRobot().getSettingsParser().getDouble("hw:oi:spare2") ;

            //
            // Actions
            //
            collect_ = mapButton(collect_b, OIButton::ButtonType::LowToHigh) ;        // Push button
            dump_ = mapButton(dump_b, OIButton::ButtonType::LowToHigh) ;              // Push button
            turtle_ = mapButton(turtle_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            eject_ = mapButton(eject_b, OIButton::ButtonType::LowToHigh) ;            // Push button
            spare1_ = mapButton(spare1_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            spare2_ = mapButton(spare2_b, OIButton::ButtonType::LowToHigh) ;          // Push button
        }

        void RanseurOIDevice::generateActions(SequenceAction &seq){
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

            button = settings.getInteger("oi:collect") ;
            collect_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            dump_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            turtle_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            eject_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            spare1_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            spare2_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;    
        
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

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
#include <actions/Action.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <tubmanipulatorsubsystem/TubManipulatorCollectAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorDumpAction.h>
#include <tubmanipulatorsubsystem/TubManipulatorEjectAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        RanseurOIDevice::RanseurOIDevice(RanseurOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            initialize() ;
        }

        RanseurOIDevice::~RanseurOIDevice() {
        }

        int RanseurOIDevice::getAutoModeSelector() {
            int ret = 0 ;

            if (getValue(automode2_))
                ret |= 2 ;
            if (getValue(automode1_))
                ret |= 1 ;

            return ret ;
        }

        void RanseurOIDevice::initialize() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            //
            // Get button numbers
            //
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:collect") ;
            size_t dump_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:dump") ;
            size_t turtle_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:turtle") ;
            size_t eject_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:eject") ;
            size_t spare1_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:spare1") ;
            size_t spare2_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:spare2") ;
            size_t automode1_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:automode:1") ;
            size_t automode2_b = getSubsystem().getRobot().getSettingsParser().getDouble("oi:automode:2") ;

            //
            // Actions
            //
            collect_ = mapButton(collect_b, OIButton::ButtonType::LowToHigh) ;        // Push button
            dump_ = mapButton(dump_b, OIButton::ButtonType::LowToHigh) ;              // Push button
            turtle_ = mapButton(turtle_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            eject_ = mapButton(eject_b, OIButton::ButtonType::LowToHigh) ;            // Push button
            spare1_ = mapButton(spare1_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            spare2_ = mapButton(spare2_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            automode1_ = mapButton(automode1_b, OIButton::ButtonType::Level) ;        // Level toggle switch
            automode2_ = mapButton(automode2_b, OIButton::ButtonType::Level) ;        // Level toggle switch
        }

        void RanseurOIDevice::generateActions(SequenceAction &seq){
          
            auto &ranseur = dynamic_cast<Ranseur &>(getSubsystem().getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;
            auto ranseurrobotsubsystem = ranseur.getRanseurRobotSubsystem() ;

            //bool ret = false ;
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
        
        /// Initializing! ///
            if (collecting_ == nullptr) {
                init() ;
            }
            if (dumping_ == nullptr) {
                init() ;
            }
            if (turtling_ == nullptr) {
                init() ;
            }
            if (ejecting_ == nullptr) {
                init() ;
            }


            //actions and buttons corresponding with the actions
            //actions found on wiki under tub manipulator subsystem under ranseur (robot)
        /// Actioning! ///
            if(getValue(collect_)) { 
                seq.pushSubActionPair(tubmanipulatorsubsytem, collecting_) ;
            }
            if(getValue(dump_)) {
                seq.pushSubActionPair(tubmanipulatorsubsytem, dumping_) ;
            }
            if(getValue(turtle_)) {
                seq.pushSubActionPair(ranseurrobotsubsystem, turtling_) ;
            }
            if(getValue(eject_)) {
                seq.pushSubActionPair(tubmanipulatorsubsytem, ejecting_) ;
            }
      
        }

        //
        // Create static actions for the OI
        //
        void RanseurOIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;      

            auto &ranseur = dynamic_cast<Ranseur &>(getSubsystem().getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;
            auto ranseurrobotsubsystem = ranseur.getRanseurRobotSubsystem() ;
            
            collecting_ = std::make_shared<TubManipulatorCollectAction>(*tubmanipulatorsubsytem) ;
            dumping_ = std::make_shared<TubManipulatorDumpAction>(*tubmanipulatorsubsytem) ;
            //turtle_ = std::make_shared<RanseurRobotSubsystem>(*ranseurrobotsubsystem) ;
            ejecting_ = std::make_shared<TubManipulatorEjectAction>(*tubmanipulatorsubsytem) ;
            
        }
    }
}

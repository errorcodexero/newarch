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
#include <ranseurrobotsubsystem/RanseurRobotTurtleAction.h>
#include <tubtoucher/TubToucherDeployAction.h>
#include <bunnyarm/BunnyArmDeployAction.h>

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

        void RanseurOIDevice::initialize() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

#ifdef RANSEUR_OLD_OI
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;
#else
            size_t automode1_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:automode1") ;
            size_t automode2_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:automode2") ;
#endif

            //
            // Get button numbers
            //
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collect") ;
            size_t dump_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:dump") ;
            size_t turtle_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:turtle") ;
            size_t eject_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:eject") ;
            size_t tubtoucher_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:tubtoucher") ;
            size_t spare2_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:spare2") ;
            size_t bunny_arm_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:bunnyarm") ;
            size_t tub_touchers_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:tubtouchers") ;

            //
            // Actions
            //
            collect_ = mapButton(collect_b, OIButton::ButtonType::LowToHigh) ;        // Push button
            dump_ = mapButton(dump_b, OIButton::ButtonType::LowToHigh) ;              // Push button
            turtle_ = mapButton(turtle_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            eject_ = mapButton(eject_b, OIButton::ButtonType::LowToHigh) ;            // Push button
            tubtoucher_ = mapButton(tubtoucher_b, OIButton::ButtonType::LowToHigh) ;  // Push button
            spare2_ = mapButton(spare2_b, OIButton::ButtonType::LowToHigh) ;          // Push button
            bunny_arm_ = mapButton(bunny_arm_b, OIButton::ButtonType::Level) ;        // Toggle switch
            tub_touchers_ = mapButton(tub_touchers_b, OIButton::ButtonType::LevelInv) ;        // Toggle switch

#ifndef RANSEUR_OLD_OI
            automode1_ = mapButton(automode1_b, OIButton::ButtonType::LevelInv) ;        // toggle switch 
            automode2_ = mapButton(automode2_b, OIButton::ButtonType::LevelInv) ;        // toggle switch
#endif
        }

        int RanseurOIDevice::getAutoModeSelector(){

#ifdef RANSEUR_OLD_OI
            return getValue(automode_) ;
#else
            int ret = 0 ;
            if(getValue(automode1_)) {
                ret |= (1 << 0) ;
            }
            if(getValue(automode2_)) {
                ret |= (1 << 1) ;
            }
            return ret ;
#endif   

        }

        void RanseurOIDevice::generateActions(SequenceAction &seq) {
          
            auto &ranseur = dynamic_cast<Ranseur &>(getSubsystem().getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubtoucher = ranseur.getRanseurRobotSubsystem()->getTubToucher() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;
            auto ranseurrobotsubsystem = ranseur.getRanseurRobotSubsystem() ;

            //actions and buttons corresponding with the actions
            //actions found on wiki under tub manipulator subsystem under ranseur (robot)
            /// Actioning! ///
            if(getValue(collect_))
            {
                if (!tubmanipulatorsubsytem->isBusy())
                    seq.pushSubActionPair(tubmanipulatorsubsytem, sequence_, false) ;
                else
                {
                    MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
                    log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_RANSEUR_OI) ;
                    log << "OI: could not execut collect, subsystem is busy" ;
                    log.endMessage() ;                    
                }
            }
            if(getValue(dump_) && !tubmanipulatorsubsytem->isBusy()) {
                seq.pushSubActionPair(tubmanipulatorsubsytem, dumping_, false) ;
            }
            if(getValue(turtle_)) {
                seq.pushSubActionPair(ranseurrobotsubsystem, turtling_, false) ;
            }
            if(getValue(eject_) && !tubmanipulatorsubsytem->isBusy()) {
                seq.pushSubActionPair(tubmanipulatorsubsytem, ejecting_, false) ;
            } 

            if (getValue(bunny_arm_) != bunnyarm->isRaised())
            {
                if (getValue(bunny_arm_))
                    seq.pushSubActionPair(bunnyarm, bunny_arm_deploy_, false) ;
                else
                    seq.pushSubActionPair(bunnyarm, bunny_arm_retract_, false) ;
            }

            if (getValue(tub_touchers_) != tubtoucher->isDeployed())
            {
                if (getValue(tub_touchers_))
                    seq.pushSubActionPair(tubtoucher, tub_touchers_deploy_, false) ;
                else
                    seq.pushSubActionPair(tubtoucher, tub_touchers_retract_, false) ;
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
            auto tubtoucher = ranseur.getRanseurRobotSubsystem()->getTubToucher() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;
            auto tubarm = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubArm() ;
            auto tubwrist = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubWrist() ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;
            auto camera = ranseur.getRanseurRobotSubsystem()->getCameraTracker() ;
            auto tubmanipulatorsubsytem = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem() ;
            auto ranseurrobotsubsystem = ranseur.getRanseurRobotSubsystem() ;
            
            bunny_arm_deploy_ = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
            bunny_arm_retract_ = std::make_shared<BunnyArmDeployAction>(*bunnyarm, false) ;            
            tub_touchers_deploy_ = std::make_shared<TubToucherDeployAction>(*tubtoucher, true) ;
            tub_touchers_retract_ = std::make_shared<TubToucherDeployAction>(*tubtoucher, false) ;                
            collecting_ = std::make_shared<TubManipulatorCollectAction>(*tubmanipulatorsubsytem) ;
            dumping_ = std::make_shared<TubManipulatorDumpAction>(*tubmanipulatorsubsytem) ;
            turtling_ = std::make_shared<RanseurRobotTurtleAction>(*ranseurrobotsubsystem) ;
            ejecting_ = std::make_shared<TubManipulatorEjectAction>(*tubmanipulatorsubsytem) ;

            sequence_ = std::make_shared<SequenceAction>(log) ;
            sequence_->pushAction(collecting_) ;
            sequence_->pushAction(dumping_) ;
            sequence_->pushAction(ejecting_) ;            
        }
    }
}

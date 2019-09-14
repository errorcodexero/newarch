#include "PhoenixOIDevice.h"
#include "PhoenixOISubsystem.h"
#include "Phoenix.h"

// Subsystems and associated actions
#include "wings/Wings.h"
#include "wings/WingsDeployAction.h"
#include "intake/Intake.h"
#include "phlifter/Lifter.h"
#include "phlifter/LifterGoToHeightAction.h"
#include "phlifter/LifterPowerAction.h"
#include "phlifter/LifterHoldClimbAction.h"
#include "phlifter/LifterCalibrateAction.h"
#include "liftingcollector/LiftingCollector.h"
#include "grabber/Grabber.h"
#include "grabber/GrabberCalibrateAction.h"
#include "grabber/GrabberToAngleAction.h"
#include "collector/Collector.h"
#include "collector/CollectCubeAction.h"
#include "collector/CollectorEjectCubeAction.h"
#include "phoenixsubsystem/PhoenixRobotSubsystem.h"

// Action related classes
#include <SequenceAction.h>


using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixOIDevice::PhoenixOIDevice(PhoenixOISubsystem &sub, int index) : OIDevice(sub, index) {
            prev_climb_enabled_ = false ;
            lifter_stop_ = false ;
            initialize() ;
        }

        PhoenixOIDevice::~PhoenixOIDevice() {
        }
        
        void PhoenixOIDevice::initialize() {
            //
            // Actions
            //
            floor_ = mapButton(1, OIButton::ButtonType::LowToHigh) ;                // Push button
            exchange_ = mapButton(2, OIButton::ButtonType::LowToHigh) ;             // Push button
            switch_ = mapButton(3, OIButton::ButtonType::LowToHigh) ;               // Push button
            scale_ = mapButton(4, OIButton::ButtonType::LowToHigh) ;                // Push button
            collect_ = mapButton(5, OIButton::ButtonType::LowToHigh) ;              // Push button
            eject_ = mapButton(6, OIButton::ButtonType::LowToHigh) ;                // Push button
            drop_ = mapButton(7, OIButton::ButtonType::LowToHigh) ;                 // Push button
            climb_ = mapButton(8, OIButton::ButtonType::LowToHigh) ;                // Push button
            wings_ =  mapButton(9, OIButton::ButtonType::LowToHigh) ;               // Momentary toggle
            eject_slow_ = mapButton(10, OIButton::ButtonType::LowToHigh) ;           // Push button
            intake_auto_ = mapButton(11, OIButton::ButtonType::Level) ;             // Toggle
            calibrate_ = mapButton(12, OIButton::ButtonType::LowToHigh) ;           // PUsh button
            lifter_up_ = mapButton(13, OIButton::ButtonType::Level) ;               // Joystick
            lifter_down_ = mapButton(14, OIButton::ButtonType::Level) ;             // Joystick
            climb_disabled_ = mapButton(15, OIButton::ButtonType::LevelInv) ;       // Toggle
            lifter_high_power_ = mapButton(16, OIButton::ButtonType::Level) ;       // Toggle

            //
            // Modes
            //
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;                                  // Rotary knob, 10 position
            grabber_auto_ = mapAxisSwitch(5, 2) ;                                   // Toggle two position
            grabber_mode_ = mapAxisSwitch(2, 3) ;                                   // Toggle three position
            intake_mode_ = mapAxisSwitch(4, 3) ;                                    // Toggle three position
            collect_end_height_ = mapAxisSwitch(3, 3) ;                             // Toggle three position

        }

        void PhoenixOIDevice::createActions() {
            Phoenix &ph = dynamic_cast<Phoenix &>(getSubsystem().getRobot()) ;
            auto lifter = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter() ;
            auto grabber = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getGrabber() ;
            auto collector = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector() ;

            //
            // Prebuilt actions
            //
            to_floor_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:floor") ;
            to_exchange_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:exchange") ;
            to_switch_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:switch") ;
            to_scale_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:scale") ;
            to_climb_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:climb") ;
            lifter_up_low_power_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:low:up") ;
            lifter_down_low_power_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:low:down") ;
            lifter_up_high_power_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:high:up") ;
            lifter_down_high_power_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:high:down") ;          
            lifter_climb_up_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:climb:up", true) ;
            lifter_climb_down_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:dutycycle:climb:down", true) ;            
            lifter_hold_climb_ = std::make_shared<LifterHoldClimbAction>(*lifter, "lifter:climbhold:delta") ;           
            lifter_calibrate_ = std::make_shared<LifterCalibrateAction>(*lifter) ;
            grabber_calibrate_ = std::make_shared<GrabberCalibrateAction>(*grabber) ;
            grabber_stow_ = std::make_shared<GrabberToAngleAction>(*grabber, "grabber:angle:stowed") ;
            collect_cube_ = std::make_shared<CollectCubeAction>(*collector) ;
            eject_normal_ = std::make_shared<CollectorEjectCubeAction>(*collector, "collector:eject:fast") ;
            eject_medium_ = std::make_shared<CollectorEjectCubeAction>(*collector, "collector:eject:middle") ;
            eject_slow_action_ = std::make_shared<CollectorEjectCubeAction>(*collector, "collector:eject:slow") ;
            lifter_zero_ = std::make_shared<LifterPowerAction>(*lifter, 0.0) ;
        }

        void PhoenixOIDevice::generateActions(SequenceAction &seq) {
            if (to_floor_ == nullptr)
                createActions() ;

            Phoenix &ph = dynamic_cast<Phoenix &>(getSubsystem().getRobot()) ;
            auto lifter = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter() ;
            auto grabber = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getGrabber() ;
            auto collector = ph.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector() ;
            auto wings = ph.getPhoenixRobotSubsystem()->getWings() ;

            if (getValue(climb_disabled_)) {
                prev_climb_enabled_ = false ;

                if (getValue(floor_)) {
                    seq.pushSubActionPair(lifter, to_floor_) ;
                    lifter_stop_ = false ;                    
                }
                else if (getValue(exchange_)) {
                    seq.pushSubActionPair(lifter, to_exchange_) ;
                    lifter_stop_ = false ;                    
                }
                else if (getValue(switch_)) {
                    seq.pushSubActionPair(lifter, to_switch_) ;
                    lifter_stop_ = false ;                    
                }
                else if (getValue(scale_)) {
                    seq.pushSubActionPair(lifter, to_scale_) ;
                    lifter_stop_ = false ;                    
                }
                else if (getValue(climb_)) {
                    seq.pushSubActionPair(lifter, to_climb_) ;
                    lifter_stop_ = false ;                    
                }
                else if (getValue(lifter_up_)) {
                    if (getValue(lifter_high_power_))
                        seq.pushSubActionPair(lifter, lifter_up_high_power_) ;
                    else
                        seq.pushSubActionPair(lifter, lifter_up_low_power_) ;

                    lifter_stop_ = true ;
                } else if (getValue(lifter_down_)) {
                    if (getValue(lifter_high_power_))
                        seq.pushSubActionPair(lifter, lifter_down_high_power_) ;
                    else
                        seq.pushSubActionPair(lifter, lifter_down_low_power_) ;                 
                        
                    lifter_stop_ = true ;
                }
                else if (lifter_stop_) {
                    //
                    // If the UP or DOWN joystick had been previously enaged, when it is
                    // released we assign a nullptr action to the lifter to stop the movements
                    //
                    seq.pushSubActionPair(lifter, lifter_zero_) ;
                    lifter_stop_ = false ;
                }


                if (getValue(calibrate_)) {
                    seq.pushSubActionPair(lifter, lifter_calibrate_) ;
                    seq.pushSubActionPair(grabber, grabber_calibrate_) ;
                }
                else if (getValue(collect_)) {
                    //
                    // TODO: This button should toggle collect mode
                    //
                    seq.pushSubActionPair(collector, collect_cube_) ;
                }
                else if (getValue(eject_)) {
                    seq.pushSubActionPair(collector, eject_normal_) ;                    
                }
                else if (getValue(eject_slow_)) {
                    seq.pushSubActionPair(collector, eject_medium_) ;  
                }
                else if (getValue(drop_)) {
                    seq.pushSubActionPair(collector, eject_slow_action_) ;                      
                }

            }
            else {
                if (prev_climb_enabled_ == false) {
                    //
                    // When the climb disabled switch is flipped to the climb enabled
                    // state, we assign an action to the grabber to stow the grabber arms
                    // out of the way
                    //
                    seq.pushSubActionPair(grabber, grabber_stow_) ;
                    prev_climb_enabled_ = true ;
                }

                if (getValue(lifter_up_)) {
                    seq.pushSubActionPair(lifter, lifter_climb_up_) ;
                    lifter_stop_ = true ;
                } else if (getValue(lifter_down_)) {
                    seq.pushSubActionPair(lifter, lifter_climb_down_) ;
                    lifter_stop_ = true ;
                }
                else {
                    seq.pushSubActionPair(lifter, lifter_hold_climb_) ;
                }

                if (getValue(wings_)) {
                    auto act = std::make_shared<WingsDeployAction>(*wings) ;
                    seq.pushSubActionPair(wings, act) ;
                }
            }
        }
    }
}
#include "TubCollector.h"
#include "TubCollectorAction.h"
#include <actions/SequenceAction.h>
#include <Robot.h>
#include <DriveBase.h>
#include <frc/DigitalInput.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/PowerDistributionPanel.h>
#include "ranseurids.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        TubCollector::TubCollector(Subsystem *parent) : Subsystem(parent, "tubcollector"), pdp_(0) {
            Robot &robot = getRobot();
            
            int sensor = robot.getSettingsParser().getInteger("hw:tubcollector:tubsensor") ;
            sensor_ = std::make_shared<frc::DigitalInput>(sensor) ;

            intake1_ = robot.getMotorFactory()->createMotor("hw:tubcollector1") ;
            intake1_->setNeutralMode(MotorController::NeutralMode::Brake) ;

            intake2_ = robot.getMotorFactory()->createMotor("hw:tubcollector2") ;
            intake2_->setNeutralMode(MotorController::NeutralMode::Brake) ;            

            int clamp = robot.getSettingsParser().getInteger("hw:tubcollector:clamp") ;
            clamp_ = std::make_shared<frc::Solenoid>(clamp) ;

            first_current_trigger_ = robot.getSettingsParser().getInteger("tubcollector:first_current_trigger") ;
            second_current_trigger_ = robot.getSettingsParser().getInteger("tubcollector:second_current_trigger") ;
            second_current_limit_ = robot.getSettingsParser().getInteger("tubcollector:second_current_limit") ;
            third_current_trigger_ = robot.getSettingsParser().getInteger("tubcollector:third_current_trigger") ;
            collector_motor_pdp_1_ = robot.getSettingsParser().getInteger("hw:tubcollector:collectmotor:pdp:1") ;
            collector_motor_pdp_2_ = robot.getSettingsParser().getInteger("hw:tubcollector:collectmotor:pdp:2") ;
            delay_time_ = robot.getSettingsParser().getDouble("tubcollector:third_state_delay_time") ;

            debounce_ = std::make_shared<DebounceBoolean>(false, 0.08) ;
            loops_ = 0 ;
            state_ = 0 ;
            has_tub_ = false ;
        }

        TubCollector::~TubCollector() {
        }

        // sees whether or not 
        bool TubCollector::canAcceptAction(ActionPtr action) {
            auto tub_col_act_p = std::dynamic_pointer_cast<TubCollectorAction>(action) ;
            if (tub_col_act_p == nullptr)
                return false ;

            return true ;
        }

        void TubCollector::computeState() {
            Subsystem::computeState() ;

            collect_power_ = (pdp_.GetCurrent(collector_motor_pdp_1_) + pdp_.GetCurrent(collector_motor_pdp_2_)) / 2.0 ;

            if (power_mode_)
            {
                auto db = getRobot().getDriveBase() ;
                assert(db != nullptr) ;
                
                if (std::fabs(db->getVelocity()) < 1.0 && !has_tub_)
                {
                    auto &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TUBCOLLECTOR) ;
                    logger << "Drive base stopped, checking for tub via sensor" ;
                    logger.endMessage() ;

                    if (!sensor_->Get())
                    {
                        logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TUBCOLLECTOR) ;
                        logger << "Note, sensor was true" ;
                        logger.endMessage() ;                        
                    }
                    
                    //
                    // Kind of a hack.  If the infrared sensor is true and we are in auto mode (where we do current sensing) and
                    // the drivebase has stopped driving, return the fact that we have a tub.
                    //
                    has_tub_ = true ;
                    return ;
                }

                switch (state_)
                {
                    case 0:
                        //
                        // Start state, looking for current to start to flow
                        //
                        has_tub_ = false ;
                        if (collect_power_ > first_current_trigger_)
                            state_ = 1 ;
                        break ;

                    case 1:
                        //
                        // We have seen the initial current spike, the motors are turning on
                        // and the current should be higher than steady state.  We want for the current
                        // to drop below the peak and under some steady state limit (but above zero)
                        //
                        has_tub_ = false ;
                        if (collect_power_ < second_current_trigger_)
                        {
                            if (collect_power_ < second_current_limit_)
                            {
                                //
                                // It fell too low, start over
                                //
                                state_ = 0 ;
                            }
                            else
                            {
                                //
                                // Motor current in its steady state
                                //
                                state_ = 2 ;
                            }
                        }
                        break ;

                    case 2:
                        //
                        // We are in the steady state, look for a spike as the current ramps
                        // as we collect a tub
                        //
                        has_tub_ = false ;
                        if (collect_power_ > third_current_trigger_)
                        {
                            state_ = 3 ;
                            start_time_ = getRobot().getTime() ;
                        }
                        else if (collect_power_ < second_current_limit_)
                        {
                            state_ = 0 ;
                        }
                        break ;

                    case 3:
                        if (getRobot().getTime() - start_time_ > delay_time_)
                        {
                            has_tub_ = true ;
                            state_ = 4 ;
                        }
                        break ;

                    case 4:
                        if (collect_power_ < second_current_limit_)
                        {
                            has_tub_ = false ;
                            state_ = 0 ;
                        }
                        break ;
                }
            }
            else
            {
                debounce_->update(!sensor_->Get(), getRobot().getTime()) ;
                has_tub_ = debounce_->get() ;
                frc::SmartDashboard::PutBoolean("HasTub", has_tub_) ;
            }
        }

        void TubCollector::init(LoopType lt)
        {
            if (lt == LoopType::Autonomous)
                power_mode_ = true ;
            else
                power_mode_ = false ;
        }

        void TubCollector::reset() {
            Subsystem::reset() ;
            intake1_->set(0.0) ;
            intake2_->set(0.0) ;
        }
    }
}

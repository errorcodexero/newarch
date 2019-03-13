#include "Lifter.h"
#include "LifterAction.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>

using namespace xero::base ;
using namespace xero::misc ;
using namespace ctre::phoenix::motorcontrol ;

namespace xero {
    namespace base {
        Lifter::Lifter(Robot &robot, uint64_t id) : Subsystem(robot, "lifter") {
            SettingsParser &parser = robot.getSettingsParser() ;

            msg_id_ = id ;
            
            getMotors(robot) ;

            int e1 = parser.getInteger("hw:lifter:encoder1");
            int e2 = parser.getInteger("hw:lifter:encoder2");
            encoder_ = std::make_shared<frc::Encoder>(e1, e2) ;
            encoder_->Reset() ;

            if (parser.isDefined("hw:lifter:limit:bottom")) {
                int lbottom = parser.getInteger("hw:lifter:limit:bottom");
                bottom_limit_ = std::make_shared<frc::DigitalInput>(lbottom) ;
            }

            if (parser.isDefined("hw:lifter:limit:top")) {
                int ltop = parser.getInteger("hw:lifter:limit:top");
                top_limit_ = std::make_shared<frc::DigitalInput>(ltop) ;
            }

            calibrate_from_limit_switch_ = false ;
            std::string limit = "lifter:calibrate_limit_switch" ;
            if (parser.isDefined(limit)) {
                if (parser.get(limit).isBoolean()) {
                    if (parser.getBoolean(limit))
                        calibrate_from_limit_switch_ = true ;
                }
            }

            //
            // The height of the lifter when it is at the bottom of travel, relative to 
            // the floor
            //
            lifter_offset = parser.getDouble("lifter:base") ;
            
            //
            // The nubmer of inches per encoder tick
            //
            inches_per_tick_ = parser.getDouble("lifter:inches_per_tick") ;

            //
            // The maximum height of the lifter independent of the limit switches
            //
            max_height_ = parser.getDouble("lifter:max_height") ;

            //
            // The minimum height of the lifter independent of limit switches
            min_height_ = parser.getDouble("lifter:min_height") ;

            calibrate(0) ;

            // Initialize to false, only gets reset if we have a limit switch
            bottom_limit_switch_ = false ;
            top_limit_switch_ = false ;
        }

        Lifter::~Lifter() {
        }

        void Lifter::getMotors(Robot &robot) {
            int i = 1 ;
            bool reverse = false ;

            std::string revname = "hw:lifter:reversemotors" ;

            if (robot.getSettingsParser().isDefined(revname)) {
                if (robot.getSettingsParser().get(revname).isBoolean()) {
                    if (robot.getSettingsParser().getBoolean(revname)) {
                        reverse = true ;
                    }
                }
            }

            while (true) {
                std::string motorname = "hw:lifter:motor:" + std::to_string(i) ;
                if (!robot.getSettingsParser().isDefined(motorname))
                    break ;

                int motor = robot.getSettingsParser().getInteger(motorname) ;
                auto talon = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(motor) ;
                talon->SetInverted(reverse) ;
                talon->ConfigVoltageCompSaturation(12.0, 10) ;
                talon->EnableVoltageCompensation(true) ;                
                talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;

                // TODO - generalize this for the lifter
                talon->ConfigForwardLimitSwitchSource(LimitSwitchSource_FeedbackConnector, LimitSwitchNormal_NormallyOpen) ;
                talon->ConfigReverseLimitSwitchSource(LimitSwitchSource_FeedbackConnector, LimitSwitchNormal_NormallyClosed) ;

                if (motors_.size() > 0)
                    talon->Follow(*motors_.front()) ;

                motors_.push_back(talon) ;

                i++ ;
            }
        }

        bool Lifter::canAcceptAction(ActionPtr action) {
            auto dir_p = std::dynamic_pointer_cast<LifterAction>(action) ;
            if (dir_p == nullptr)
                return false ;

            return true ;
        }

        void Lifter::setMotorPower(double v) {
            if (is_calibrated_) {
                //
                // If we are calibrated, we know our height.  If our height exceeds
                // the maximum allow height and we are trying to move up, we set the
                // power to zero
                //
                if (v < 0 && height_ <= min_height_)
                    v = 0.0 ;
                else if (v > 0 && height_ >= max_height_)
                    v = 0.0 ;
            }

            //
            // Now check the raw limit switch states
            //
            if (v > 0 && isAtTop())
                v = 0.0 ;
            else if (v < 0 && isAtBottom())
                v = 0.0 ;

            power_ = v ;
            motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v);
        }

        void Lifter::computeState() {
            encoder_value_ = encoder_->Get() ;
            if (bottom_limit_ != nullptr)
                bottom_limit_switch_ = !bottom_limit_->Get() ;

            if (top_limit_ != nullptr)
                top_limit_switch_ = !top_limit_->Get() ;

            if (is_calibrated_) {
                height_ = (encoder_value_ + encoder_base_) * inches_per_tick_ + lifter_offset ;
                speed_ = (height_ - last_height_) / getRobot().getDeltaTime() ;
                last_height_ = height_ ;
            }

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, msg_id_) ;
            logger << "Lifter:" ;
            logger << " ticks " << encoder_value_ ;
            logger << " encbase " << encoder_base_ ;
            logger << " calibrated " << is_calibrated_ ;
            logger << " height " << height_ ;
            logger << " speed " << speed_ ;
            logger << " power " << power_ ;
            logger.endMessage() ;

            frc::SmartDashboard::PutNumber("Lift", height_) ;              
        }

        void Lifter::calibrate(int encbase) {
            is_calibrated_ = true ;
            encoder_->Reset() ;
            encoder_base_ = encbase ;
            speed_ = 0.0 ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::info) ;
            logger << "Lifter: calibrated with encoder base " << encbase ;
            logger << " encoders is " << encoder_->Get() ;
            logger.endMessage() ;            
        }
    }
}

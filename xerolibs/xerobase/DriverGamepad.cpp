#include "DriverGamepad.h"
#include "OISubsystem.h"
#include "Robot.h"
#include "ActionSequence.h"
#include "basegroups.h"
#include "tankdrive/TankDrive.h"
#include "tankdrive/TankDrivePowerAction.h"
#include "tankdrive/TankDriveTimedPowerAction.h"
#include <MessageLogger.h>
#include <frc/DriverStation.h>
#include <frc/GenericHID.h>
#include <cmath>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        DriverGamepad::DriverGamepad(OISubsystem &oi, int index): HIDDevice(oi, index) {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            if (ds.GetStickPOVCount(getIndex()) == 0) {
                MessageLogger &logger = oi.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning, MSG_GROUP_OI) ;
                logger << "driver gamepad does not have POVs, nudged will be disabled" ;
                logger.endMessage() ;
                pov_ = -1 ;
            }
            else {
                pov_ = 0 ;
            }

            if (ds.GetStickAxisCount(getIndex()) < 6) {
                MessageLogger &logger = oi.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_OI) ;
                logger << "driver gamepad does not have six axis (Left, Right, Buttons), cannot use as driver gamepad" ;
                logger.endMessage() ;                
            }

            controller_ = std::make_shared<frc::XboxController>(index) ;
        }

        DriverGamepad::~DriverGamepad() {            
        }

        void DriverGamepad::rumble(bool left, double value) {
            if (controller_ != nullptr) {
                frc::XboxController::RumbleType rtype = frc::XboxController::RumbleType::kLeftRumble ;

                if (!left)
                    rtype = frc::XboxController::RumbleType::kRightRumble ;

                controller_->SetRumble(rtype, value) ;
            }
        }

        void DriverGamepad::init(std::shared_ptr<TankDrive> db) {
            db_ = db ;

            default_duty_cycle_ = getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:default") ;
            max_duty_cycle_ = getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:max") ;
            slow_factor_= getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:slowby") ;

            tolerance_ = getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:tolerance") ;

            double nudge_straight = getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:nudge_straight") ;
            double nudge_rotate = getSubsystem().getRobot().getSettingsParser().getDouble("driver:power:nudge_rotate") ;
            double nudge_time = getSubsystem().getRobot().getSettingsParser().getDouble("driver:nudge_time") ;

            nudge_forward_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_straight, nudge_straight, nudge_time) ;
            nudge_backward_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_straight, -nudge_straight, nudge_time) ;
            nudge_clockwise_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_rotate, nudge_rotate, nudge_time) ;
            nudge_counter_clockwise_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_rotate, -nudge_rotate, nudge_time) ;
        }

        double DriverGamepad::scalePower(double axis, double boost, bool slow) {
            //
            // The boost value (0 - 1) picks a power between DEFAULT and MAX
            //
            double baseduty = default_duty_cycle_ + (max_duty_cycle_ - default_duty_cycle_) * boost ;

            //
            // We slow down by this amount.  Generally we dont expect boost to be non-zewro and slow be true
            //
            double slowdown = slow ? default_duty_cycle_ * slow_factor_ : 0.0 ;

            //
            // Combine them for the final duty cycle
            //
            return axis * (baseduty - slowdown) ;
        }

        void DriverGamepad::computeState() {
        }

        void DriverGamepad::generateActions(ActionSequence &seq) {
            int pov ;

            if (nudge_forward_ == nullptr)
                init(db_) ;

            if (db_ == nullptr) {
                auto &logger = getSubsystem().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger << "DriverGamepad: generateActions called with no attached drivebase" ;
                logger.endMessage() ;
            }

            if (getIndex() == -1)
                return ;

            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            if (pov_ != -1)
                pov = ds.GetStickPOV(getIndex(), pov_) ;
            else
                pov = POVAngle::NONE ;

            double ly = ds.GetStickAxis(getIndex(),AxisNumber::LEFTY) ;
            double rx = ds.GetStickAxis(getIndex(),AxisNumber::RIGHTX) ;

            if (pov == POVAngle::LEFT) {
                seq.pushSubActionPair(db_, nudge_clockwise_, false) ;
            }
            else if (pov == POVAngle::UP) {
                seq.pushSubActionPair(db_, nudge_forward_, false) ;
            }
            else if (pov == POVAngle::DOWN) {
                seq.pushSubActionPair(db_, nudge_backward_, false) ;
            }
            else if (pov == POVAngle::RIGHT) {
                seq.pushSubActionPair(db_, nudge_counter_clockwise_, false) ;
            }
            else {
                bool slow = ds.GetStickButton(getIndex(),ButtonNumber::LB) ;
                double boost = ds.GetStickAxis(getIndex(),AxisNumber::LTRIGGER) ;

                double power = scalePower(-ly, boost, slow) ;
                double spin = (std::fabs(rx) > 0.01) ? scalePower(rx, boost, slow) : 0.0 ;
                
                if (std::fabs(power + spin - left_) > tolerance_ || std::fabs(power - spin - right_) > tolerance_) {
                    auto dir = std::make_shared<TankDrivePowerAction>(*db_, power + spin, power - spin) ;
                    seq.pushSubActionPair(db_, dir) ;
                    left_ = power + spin ;
                    right_ = power - spin ;
                }
            }
        }
    }
}
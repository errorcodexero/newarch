#include "oi/DriverGamepad.h"
#include "oi/OISubsystem.h"
#include "Robot.h"
#include "actions/SequenceAction.h"
#include "basegroups.h"
#include "tankdrive/TankDrive.h"
#include "tankdrive/actions/TankDrivePowerAction.h"
#include "tankdrive/actions/TankDriveTimedPowerAction.h"
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
            reverse_ = false ;
            high_gear_ = true ;
            rumbling_ = false ;
            left_ = std::numeric_limits<double>::max() ;
            right_ = std::numeric_limits<double>::max() ;

            auto &parser = oi.getRobot().getSettingsParser() ;
            if (parser.isDefined("driver:spin:reverse")) {
                reverse_ = parser.getBoolean("driver:spin:reverse") ;
            }
        }

        DriverGamepad::~DriverGamepad() {            
        }

        void DriverGamepad::rumble(bool left, double value, double duration) {
            if (controller_ != nullptr) {
                rtype_ = frc::XboxController::RumbleType::kLeftRumble ;

                if (!left)
                    rtype_ = frc::XboxController::RumbleType::kRightRumble ;

                controller_->SetRumble(rtype_, value) ;
                rumbling_ = true ;
                start_ = getSubsystem().getRobot().getTime() ;
                duration_ = duration ;
            }
        }

        void DriverGamepad::init(std::shared_ptr<TankDrive> db) {
            db_ = db ;
            auto &settings = getSubsystem().getRobot().getSettingsParser() ;

            default_duty_cycle_ = settings.getDouble("driver:power:default") ;
            max_duty_cycle_ = settings.getDouble("driver:power:max") ;
            turn_default_duty_cycle_ = settings.getDouble("driver:turn:default") ;
            turn_max_duty_cycle_ = settings.getDouble("driver:turn:max") ;            
            slow_factor_= settings.getDouble("driver:power:slowby") ;
            zerolevel_ = settings.getDouble("driver:zerolevel") ;

            tolerance_ = settings.getDouble("driver:power:tolerance") ;

            double nudge_straight = settings.getDouble("driver:power:nudge_straight") ;
            double nudge_rotate = settings.getDouble("driver:power:nudge_rotate") ;
            double nudge_time = settings.getDouble("driver:nudge_time") ;

            if (settings.isDefined("driver:usepoofs"))
                use_poofs_ = settings.getBoolean("driver:usepoofs") ;

            nudge_forward_low_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_straight, nudge_straight, nudge_time, true) ;
            nudge_backward_low_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_straight, -nudge_straight, nudge_time, true) ;
            nudge_clockwise_low_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_rotate, nudge_rotate, nudge_time, true) ;
            nudge_counter_clockwise_low_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_rotate, -nudge_rotate, nudge_time, true) ;

            nudge_forward_high_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_straight, nudge_straight, nudge_time, false) ;
            nudge_backward_high_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_straight, -nudge_straight, nudge_time, false) ;
            nudge_clockwise_high_ = std::make_shared<TankDriveTimedPowerAction>(*db_, -nudge_rotate, nudge_rotate, nudge_time, false) ;
            nudge_counter_clockwise_high_ = std::make_shared<TankDriveTimedPowerAction>(*db_, nudge_rotate, -nudge_rotate, nudge_time, false) ;            
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

        double DriverGamepad::scaleTurn(double axis, double boost, bool slow) {
            //
            // The boost value (0 - 1) picks a power between DEFAULT and MAX
            //
            double baseduty = turn_default_duty_cycle_ + (turn_max_duty_cycle_ - turn_default_duty_cycle_) * boost ;

            //
            // We slow down by this amount.  Generally we dont expect boost to be non-zewro and slow be true
            //
            double slowdown = slow ? turn_default_duty_cycle_ * slow_factor_ : 0.0 ;

            //
            // Combine them for the final duty cycle
            //
            return axis * (baseduty - slowdown) ;
        }        

        void DriverGamepad::computeState() {
            if (rumbling_) {
                if (getSubsystem().getRobot().getTime() - start_ > duration_) {
                    //
                    // We are strting the rumble off side of the cycle
                    //
                    controller_->SetRumble(rtype_, 0.0) ;
                    rumbling_ = false ;
                }
            }
        }

        bool DriverGamepad::isCancelPressed() {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            return ds.GetStickButton(getIndex(), ButtonNumber::A) ;
        }

        void DriverGamepad::generateActions(SequenceAction &seq) {
            int pov ;
            auto &logger = getSubsystem().getRobot().getMessageLogger() ;

            if (db_ == nullptr) {
                logger.startMessage(MessageLogger::MessageType::warning) ;
                logger << "DriverGamepad: generateActions called with no attached drivebase" ;
                logger.endMessage() ;
            }            

            if (nudge_forward_low_ == nullptr)
                init(db_) ;

            if (getIndex() == -1)
                return ;

            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            if (pov_ != -1)
                pov = ds.GetStickPOV(getIndex(), pov_) ;
            else
                pov = POVAngle::NONE ;

            if (ds.GetStickButton(getIndex(), ButtonNumber::LB)) {
                high_gear_ = false ;
            }
            else if (ds.GetStickButton(getIndex(), ButtonNumber::RB)) {
                high_gear_ = true ;
            }

            double ly = -ds.GetStickAxis(getIndex(),AxisNumber::LEFTY) ;
            double rx = ds.GetStickAxis(getIndex(),AxisNumber::RIGHTX) ;

            if (pov == POVAngle::LEFT) {
                if (high_gear_)
                    seq.pushSubActionPair(db_, nudge_clockwise_high_, false) ;                
                else
                    seq.pushSubActionPair(db_, nudge_clockwise_low_, false) ;
            }
            else if (pov == POVAngle::UP) {
                if (high_gear_)                
                    seq.pushSubActionPair(db_, nudge_forward_high_, false) ;
                else
                    seq.pushSubActionPair(db_, nudge_forward_low_, false) ;                
            }
            else if (pov == POVAngle::DOWN) {
                if (high_gear_)                
                    seq.pushSubActionPair(db_, nudge_backward_high_, false) ;
                else
                    seq.pushSubActionPair(db_, nudge_backward_low_, false) ;                
            }
            else if (pov == POVAngle::RIGHT) {
                if (high_gear_)                
                    seq.pushSubActionPair(db_, nudge_counter_clockwise_high_, false) ;
                else
                    seq.pushSubActionPair(db_, nudge_counter_clockwise_low_, false) ;                
            }
            else {
                double left, right ;

                if (std::fabs(ly) < zerolevel_ && std::fabs(rx) < zerolevel_) {
                    left = 0.0 ;
                    right = 0.0 ;
                }
                else {
                    if (use_poofs_)
                        calcLeftRightPoofs(ly, rx, left, right) ;
                    else
                        calcLeftRightDefault(ly, rx, left, right) ;
                }
                
                if (std::fabs(left - left_) > tolerance_ || std::fabs(right - right_) > tolerance_) {
                    auto dir = std::make_shared<TankDrivePowerAction>(*db_, left, right, high_gear_) ;
                    seq.pushSubActionPair(db_, dir) ;
                    left_ = left ;
                    right_ = right ;
                }
            }
        }

        void DriverGamepad::calcLeftRightPoofs(double power, double wheel, double &left, double &right)
        {
            double kWheelGain = 0.05 ;
            double kWheelNonlinearity = 0.05 ;
            double kRobotWidth = 30.0 ;
            double kRobotScrub = 0.98 ;

            double denom = std::sin(xero::math::PI / 2.0 * kWheelGain) ;

            wheel = std::sin(xero::math::PI / 2.0 * kWheelNonlinearity * wheel) ;
            wheel = std::sin(xero::math::PI / 2.0 * kWheelNonlinearity * wheel) ;
            wheel = wheel / (denom * denom) * std::fabs(power) ;
            wheel = wheel * kWheelGain ;

            // Twist2d, x = power, y = 0, theta = wheel
            if (wheel < 0.0001)
            {
                left = power ;
                right = power ;
            }   
            else
            {
                double dv = kRobotWidth * wheel / (2.0 * kRobotScrub) ;
                left = power + dv ;
                right = power - dv ;
            }

            double scale = std::max(1.0, std::max(std::fabs(left), std::fabs(right))) ;
            left = left / scale ;
            right = right / scale ;
        }

        void DriverGamepad::calcLeftRightDefault(double power, double wheel, double &left, double &right)
        {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;

            bool slow = ds.GetStickButton(getIndex(),ButtonNumber::LB) ;
            double boost = ds.GetStickAxis(getIndex(),AxisNumber::LTRIGGER) ;

            double pwr = scalePower(power, boost, slow) ;
            double spin = (std::fabs(wheel) > 0.01) ? scaleTurn(wheel, boost, slow) : 0.0 ;

            if (reverse_) {
                left = pwr - spin ;
                right = pwr + spin ;
            }
            else {
                left = pwr + spin ;
                right = pwr - spin ;
            }
        }
    }
}

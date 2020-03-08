#include <Robot.h>

#include "MotorEncoderSubsystem.h"
#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderHoldAction.h"
#include "XeroEncoder.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc;

namespace xero {
    namespace base {
        MotorEncoderSubsystem::MotorEncoderSubsystem(Subsystem *parent, 
                                                     const std::string &name,
                                                     uint64_t id,
                                                     int samples,
                                                     bool angular)
                    : SingleMotorSubsystem(parent, name, id), 
                      speedometer_(samples, angular), msg_id_(id) 
        {
            auto &robot = getRobot(); 
            std::string encname = Subsystem::HWPrefix + name + ":encoder" ;
            encoder_ = std::make_shared<XeroEncoder>(robot, encname, angular, getMotorController()) ;
        }

        void MotorEncoderSubsystem::run()
        {
            SingleMotorSubsystem::run();
        }

        void MotorEncoderSubsystem::postHWInit()
        {
            SingleMotorSubsystem::postHWInit() ;
            if (getDefaultAction() == nullptr)
                setDefaultAction(std::make_shared<MotorEncoderHoldAction>(*this));
        }

        bool MotorEncoderSubsystem::canAcceptAction(xero::base::ActionPtr action) 
        {
            if (SingleMotorSubsystem::canAcceptAction(action))
                return true ;

            auto mot_enc_act_p = std::dynamic_pointer_cast<MotorEncoderSubsystemAction>(action);
            return (mot_enc_act_p != nullptr);
        }

        bool MotorEncoderSubsystem::canAcceptDefaultAction(xero::base::ActionPtr action) 
        { 
            return std::dynamic_pointer_cast<MotorEncoderHoldAction>(action) != nullptr;
        }

        void MotorEncoderSubsystem::computeState() 
        {
            SingleMotorSubsystem::computeState();

            double pos = encoder_->getPosition() ;
            
            speedometer_.update(getRobot().getDeltaTime(), pos) ;
            if (smartDashboardName_.length() > 0)
            {
                if (!smart_dashboard_when_disabled_ || getRobot().IsDisabled())
                    frc::SmartDashboard::PutNumber(smartDashboardName_, pos) ;
            }

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(xero::misc::MessageLogger::MessageType::debug, msg_id_) ;
            logger << getName() << ":" ;
            logger << " pos " << pos ;
            logger << " vel " << speedometer_.getVelocity() ;
            logger << " accel " << speedometer_.getAcceleration() ;
            logger.endMessage() ;
        }

        void MotorEncoderSubsystem::reset() {
            encoder_->reset() ;
            SingleMotorSubsystem::reset() ;
        }

        void MotorEncoderSubsystem::calibrate()
        {
            //
            // Assumes a complex encoder with both an absolute and a quadrature
            // position indicator.
            //
            encoder_->calibrate();
        }

        void MotorEncoderSubsystem::calibrate(double pos)
        {
            //
            // Assumes a quadrature encoder only where the current robot
            // position is supplied from the outside world some how.  This is usually
            // by assuming the robot is setup in a pre agreed position, or running a
            // mechanism until it hits a hard stop or limit switch and then knowing
            // the position at that point.
            //
            encoder_->calibrate(pos);
        }
    }
}
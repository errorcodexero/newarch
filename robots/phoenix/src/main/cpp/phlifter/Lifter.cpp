#include "Lifter.h"
#include "LifterAction.h"
#include "phoenixids.h"
#include <Robot.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Lifter::Lifter(Subsystem *parent) : MotorEncoderSubsystem(parent, "lifter", MSG_GROUP_LIFTER) 
        {
            int idx;

            idx = getRobot().getSettingsParser().getInteger("hw:lifter:shifter");
            gear_box_ = std::make_shared<frc::Solenoid>(idx);

            idx = getRobot().getSettingsParser().getInteger("hw:lifter:brake");
            brake_ = std::make_shared<frc::Solenoid>(idx);

            idx = getRobot().getSettingsParser().getInteger("hw:lifter:limit:bottom");
            bottom_limit_ = std::make_shared<frc::DigitalInput>(idx);

            idx = getRobot().getSettingsParser().getInteger("hw:lifter:limit:top");
            top_limit_ = std::make_shared<frc::DigitalInput>(idx);            

            floor_ = getRobot().getSettingsParser().getDouble("lifter:height:floor");

            // Start in high gear
            setHighGear() ;

            // Start with the brake on
            setBrakeOn() ;

            // And we start without calibration
            is_calibrated_ = false ;

            setSmartDashboardName("lifter");
        }

        Lifter::~Lifter() {
        }

        bool Lifter::canAcceptAction(ActionPtr action)
        {
            if (MotorEncoderSubsystem::canAcceptAction(action))
                return true;

            std::shared_ptr<LifterAction> lact = std::dynamic_pointer_cast<LifterAction>(action);
            if (lact == nullptr)
                return false;

            return true;
        }

        void Lifter::init(LoopType ltype)
        {
            if (ltype == LoopType::Autonomous)
                calibrate(floor_);
        }

        void Lifter::computeState() 
        {
            MotorEncoderSubsystem::computeState();

            bottom_limit_hit_ = !bottom_limit_->Get();
            top_limit_hit_ = !top_limit_->Get();
        }

        void Lifter::setLowGear() 
        {
            if (high_gear_) {
                gear_box_->Set(true) ;
                high_gear_ = false ;
                is_calibrated_ = false ;
            }
        }

        void Lifter::run()
        {
            auto &logger = getRobot().getMessageLogger();
            MotorEncoderSubsystem::run();

            double power = getMotor();
            if (power < 0 && bottom_limit_hit_)
            {
                logger.startMessage(MessageLogger::MessageType::warning);
                logger << "stopping motors - bottom limit switch detected";
                logger.endMessage();
                setMotor(0.0);
            }
            else if (power > 0 && top_limit_hit_)
            {
                logger.startMessage(MessageLogger::MessageType::warning);
                logger << "stopping motors - top limit switch detected";
                logger.endMessage();
                setMotor(0.0);
            }
        }

        void Lifter::setHighGear() 
        {
            if (!high_gear_) {
                gear_box_->Set(false) ;
                high_gear_ = true ;
                is_calibrated_ = false ;
            }
        }

        void Lifter::setBrakeOn() 
        {
            if (!brake_on_) {
                brake_->Set(false) ;
                brake_on_ = true ;
            }
        }

        void Lifter::setBrakeOff() 
        {
            if (brake_on_) {
                brake_->Set(true) ;
                brake_on_ = false ;
            }
        }
    }
}

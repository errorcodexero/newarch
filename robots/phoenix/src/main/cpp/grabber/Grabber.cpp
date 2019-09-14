#include "Grabber.h"
#include "GrabberToAngleAction.h"
#include "GrabberCalibrateAction.h"
#include "GrabberHoldCubeAction.h"
#include "phoenixgroups.h"
#include <Robot.h>
#include <SequenceAction.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Grabber::Grabber(xero::base::Robot &robot) : Subsystem(robot, "grabber") {
            int motor = robot.getSettingsParser().getInteger("hw:grabber:motor") ;
            int enc1 = robot.getSettingsParser().getInteger("hw:grabber:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:grabber:encoder2") ;

            min_angle_ = robot.getSettingsParser().getDouble("grabber:angle:minimum") ;
            max_angle_ = robot.getSettingsParser().getDouble("grabber:angle:maximum") ;
            
            motor_ = std::make_shared<frc::VictorSP>(motor) ;
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;
            degrees_per_tick_ = robot.getSettingsParser().getDouble("grabber:degrees_per_tick") ;
        }   

        Grabber::~Grabber() {
        }   

        void Grabber::calibrate() {
            encoder_->Reset() ;
            encoder_zero_ = encoder_->Get() ;
            calibrated_ = true ;
        }

        void Grabber::createNamedSequences() {
        }

        bool Grabber::canAcceptAction(xero::base::ActionPtr action) {
            MessageLogger &logger = getRobot().getMessageLogger() ;
            auto dir_goto_p = std::dynamic_pointer_cast<GrabberToAngleAction>(action) ;            
            if (dir_goto_p != nullptr) {
                if (calibrated_)
                {
                    //
                    // We only accept this if we are calibrated
                    //
                    return true ;
                }
                else {
                    logger.startMessage(MessageLogger::MessageType::warning, MSG_GROUP_GRABBER) ;
                    logger << "attempt to set GrabberToAngleAction while not calibrated" ;
                    logger.endMessage() ;
                }
            }

            auto dir_other_p = std::dynamic_pointer_cast<GrabberAction>(action) ;
            if (dir_other_p != nullptr)
                return true ;

            return false ;
        }  

        void Grabber::computeState() {
            angle_ = (encoder_->Get() - encoder_zero_) * degrees_per_tick_ ;
        }

        void Grabber::setMotorValue(double value) {
            if (calibrated_) {
                if (angle_ < min_angle_ && value < 0.0)
                    value = 0.0 ;
                else if (angle_ > max_angle_ && value > 0.0)
                    value = 0.0 ;
            }

            motor_->Set(value) ;
        }
    }
}

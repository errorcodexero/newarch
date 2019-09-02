#include "TankDriveAngleCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        TankDriveAngleCharAction::TankDriveAngleCharAction(TankDrive &drive, double duration, double start, double stop, double incr) : TankDriveAction(drive) {
            duration_ = duration ;
            start_ = start ;
            stop_ = stop ;
            incr_ = incr ;
        }

        TankDriveAngleCharAction::~TankDriveAngleCharAction() {         
        }

        void TankDriveAngleCharAction::start() {
            is_done_ = false ;
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            current_ = start_ ;
            setMotorsToPercents(current_, -current_) ;
        }

        void TankDriveAngleCharAction::run() {
            auto &logger = getTankDrive().getRobot().getMessageLogger() ;

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    logger.startMessage(MessageLogger::MessageType::info) ;
                    logger << "duration " << duration_ ;
                    logger << ", percent " << current_ ;
                    logger << ", angular velocity " << getTankDrive().getAngularVelocity() ;
                    logger << ", angular acceleration " << getTankDrive().getAngularAcceleration() ;
                    logger.endMessage() ;

                    current_ = current_ + incr_ ;
                    if (current_ > stop_) {
                        is_done_ = true ;
                    }
                    else {
                        start_time_ = frc::Timer::GetFPGATimestamp() ;
                        setMotorsToPercents(current_, -current_) ;
                    }
                }
            }
            else
            {
                setMotorsToPercents(0.0, 0.0) ;
            }
        }

        void TankDriveAngleCharAction::cancel() {
            is_done_ = true ;
            setMotorsToPercents(0, 0) ;
        }
        bool TankDriveAngleCharAction::isDone() {
            return is_done_ ;
        }

        std::string TankDriveAngleCharAction::toString() {
            std::string ret("TankDriveAngleCharAction") ;
            return ret ;
        }
    }
}

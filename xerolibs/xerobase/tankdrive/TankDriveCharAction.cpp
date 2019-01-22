#include "TankDriveCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        TankDriveCharAction::TankDriveCharAction(TankDrive &drive, double duration, double voltage, bool highgear) : TankDriveAction(drive) {
            duration_ = duration ;
            voltage_ = voltage ;
            high_gear_ = highgear ;
        }

        TankDriveCharAction::TankDriveCharAction(TankDrive &drive, const std::string &name, const std::string &voltage, bool highgear) : TankDriveAction(drive) {
            duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(name) ;
            voltage_ = getTankDrive().getRobot().getSettingsParser().getDouble(voltage) ;
            high_gear_ = highgear ;
        }       
        
        TankDriveCharAction::~TankDriveCharAction() {           
        }

        void TankDriveCharAction::start() {
            is_done_ = false ;
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            if (high_gear_)
                getTankDrive().highGear() ;
            else
                getTankDrive().lowGear() ;
                
            getTankDrive().setMotorsToPercents(voltage_, voltage_) ;
            auto &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;            
            logger << "Time,Distance,Velocity,Acceleration,TicksL,TicksR" ;
            logger.endMessage() ;

            index_ = 0 ;
            getTankDrive().getRobot().startPlot(toString(), 7) ;            
        }

        void TankDriveCharAction::run() {
            Robot &rb = getTankDrive().getRobot() ;            

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    is_done_ = true ;
                    getTankDrive().setMotorsToPercents(0.0, 0.0) ;
                    rb.endPlot(toString()) ;
                } else {
                    auto &logger = getTankDrive().getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                    logger << (now - start_time_) ;
                    logger << ", " << getTankDrive().getDist() ;
                    logger << ", " << getTankDrive().getVelocity() ;
                    logger << ", " << getTankDrive().getAcceleration() ;
                    logger << ", " << getTankDrive().getLeftTickCount() ;
                    logger << ", " << getTankDrive().getRightTickCount() ;
                    logger << ", " << voltage_ ;
                    logger.endMessage() ;

                    rb.addPlotData(toString(), index_, "time", now - start_time_) ;
                    rb.addPlotData(toString(), index_, "dist", getTankDrive().getDist()) ;
                    rb.addPlotData(toString(), index_, "velocity", getTankDrive().getVelocity()) ;
                    rb.addPlotData(toString(), index_, "acceleration", getTankDrive().getAcceleration()) ;
                    rb.addPlotData(toString(), index_, "ltick", getTankDrive().getLeftTickCount()) ;
                    rb.addPlotData(toString(), index_, "rtick", getTankDrive().getRightTickCount()) ;
                    rb.addPlotData(toString(), index_, "power", voltage_) ;

                    index_++ ;

                }
            }
        }

        void TankDriveCharAction::cancel() {
            is_done_ = true ;
        }
        bool TankDriveCharAction::isDone() {
            return is_done_ ;
        }

        std::string TankDriveCharAction::toString() {
            std::string ret("TankDriveCharAction") ;
            return ret ;
        }
    }
}

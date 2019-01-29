#include "TankDriveCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::list<std::string> TankDriveCharAction::plot_columns_ = { "time", "dist", "velocity", "acceleration", "ltick", "rtick", "power" } ;

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
            if (getTankDrive().hasGearShifter())
            {
                if (high_gear_)
                    getTankDrive().highGear() ;
                else
                    getTankDrive().lowGear() ;
            }   
            setMotorsToPercents(voltage_, voltage_) ;
            auto &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;            
            logger << "Time,Distance,Velocity,Acceleration,TicksL,TicksR" ;
            logger.endMessage() ;

            index_ = 0 ;
            plotid_ = getTankDrive().getRobot().startPlot(toString(), plot_columns_) ;           
        }

        void TankDriveCharAction::run() {
            Robot &rb = getTankDrive().getRobot() ;            

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    is_done_ = true ;
                    setMotorsToPercents(0.0, 0.0) ;
                    rb.endPlot(plotid_) ;
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

                    rb.addPlotData(plotid_, index_, 0, now - start_time_) ;
                    rb.addPlotData(plotid_, index_, 1, getTankDrive().getDist()) ;
                    rb.addPlotData(plotid_, index_, 2, getTankDrive().getVelocity()) ;
                    rb.addPlotData(plotid_, index_, 3, getTankDrive().getAcceleration()) ;
                    rb.addPlotData(plotid_, index_, 4, getTankDrive().getLeftTickCount()) ;
                    rb.addPlotData(plotid_, index_, 5, getTankDrive().getRightTickCount()) ;
                    rb.addPlotData(plotid_, index_, 6, voltage_) ;

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

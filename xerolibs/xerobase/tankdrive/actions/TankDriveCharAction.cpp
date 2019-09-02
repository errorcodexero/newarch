#include "TankDriveCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> TankDriveCharAction::plot_columns_ = { "time", "dist", "velocity", "acceleration", "ltick", "rtick", "power" } ;

        TankDriveCharAction::TankDriveCharAction(TankDrive &drive, double duration, double voltage, bool highgear) : TankDriveAction(drive) {
            duration_ = duration ;
            voltage_ = voltage ;
            high_gear_ = highgear ;

            plot_id_ = getTankDrive().getRobot().initPlot(toString()) ;
        }

        TankDriveCharAction::TankDriveCharAction(TankDrive &drive, const std::string &name, const std::string &voltage, bool highgear) : TankDriveAction(drive) {
            duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(name) ;
            voltage_ = getTankDrive().getRobot().getSettingsParser().getDouble(voltage) ;
            high_gear_ = highgear ;

            plot_id_ = getTankDrive().getRobot().initPlot(toString()) ;
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
            getTankDrive().getRobot().startPlot(plot_id_, plot_columns_) ;           
        }

        void TankDriveCharAction::run() {
            std::vector<double> data ;
            Robot &rb = getTankDrive().getRobot() ;            

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    is_done_ = true ;
                    setMotorsToPercents(0.0, 0.0) ;
                    rb.endPlot(plot_id_) ;
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

                    data.push_back(now - start_time_) ;
                    data.push_back(getTankDrive().getDist()) ;
                    data.push_back(getTankDrive().getVelocity()) ;
                    data.push_back(getTankDrive().getAcceleration()) ;
                    data.push_back(getTankDrive().getLeftTickCount()) ;
                    data.push_back(getTankDrive().getRightTickCount()) ;
                    data.push_back(voltage_) ;
                    getTankDrive().getRobot().addPlotData(plot_id_, data) ;

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

#include "TankDriveScrubCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        TankDriveScrubCharAction::TankDriveScrubCharAction(TankDrive &drive, double duration, double lpower, double rpower, bool highgear) : TankDriveAction(drive) {
            duration_ = duration ;
            lvoltage_ = lpower ;
            rvoltage_ = rpower ;
            high_gear_ = highgear ;
        }

        TankDriveScrubCharAction::~TankDriveScrubCharAction() {           
        }

        void TankDriveScrubCharAction::start() {
            is_done_ = false ;
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            if (high_gear_)
                getTankDrive().highGear() ;
            else
                getTankDrive().lowGear() ;
                
            getTankDrive().setMotorsToPercents(lvoltage_, rvoltage_) ;

            index_ = 0 ;
            getTankDrive().getRobot().startPlot(toString(), 8) ;            
        }

        void TankDriveScrubCharAction::run() {
            Robot &rb = getTankDrive().getRobot() ;            

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    is_done_ = true ;
                    getTankDrive().setMotorsToPercents(0.0, 0.0) ;
                    rb.endPlot(toString()) ;
                } else {
                    rb.addPlotData(toString(), index_, "time", now - start_time_) ;
                    rb.addPlotData(toString(), index_, "ldist", getTankDrive().getLeftDistance()) ;
                    rb.addPlotData(toString(), index_, "rdist", getTankDrive().getRightDistance()) ;
                    rb.addPlotData(toString(), index_, "lvel", getTankDrive().getLeftVelocity()) ;
                    rb.addPlotData(toString(), index_, "rvel", getTankDrive().getRightVelocity()) ;
                    rb.addPlotData(toString(), index_, "angle", getTankDrive().getAngle()) ;
                    rb.addPlotData(toString(), index_, "lpower", lvoltage_) ;
                    rb.addPlotData(toString(), index_, "rpower", rvoltage_) ;                    
                    index_++ ;
                }
            }
        }

        void TankDriveScrubCharAction::cancel() {
            is_done_ = true ;
        }
        bool TankDriveScrubCharAction::isDone() {
            return is_done_ ;
        }

        std::string TankDriveScrubCharAction::toString() {
            std::string ret("TankDriveScrubCharAction") ;
            return ret ;
        }
    }
}

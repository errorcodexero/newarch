#include "TankDriveScrubCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {

        std::list<std::string> TankDriveScrubCharAction::plot_columns_ = {
            "time", "ldist", "rdist", "lvel", "rvel", "angle", "lpower", "rpower"
        } ;

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
                
            setMotorsToPercents(lvoltage_, rvoltage_) ;

            index_ = 0 ;
            plotid_ = getTankDrive().getRobot().startPlot(toString(), plot_columns_) ;
        }

        void TankDriveScrubCharAction::run() {
            Robot &rb = getTankDrive().getRobot() ;            

            if (!is_done_) {
                double now = frc::Timer::GetFPGATimestamp() ;
                if (now - start_time_ >= duration_) {
                    is_done_ = true ;
                    setMotorsToPercents(0.0, 0.0) ;
                    rb.endPlot(plotid_) ;
                } else {
                    rb.addPlotData(plotid_, index_, 0, now - start_time_) ;
                    rb.addPlotData(plotid_, index_, 1, getTankDrive().getLeftDistance()) ;
                    rb.addPlotData(plotid_, index_, 2, getTankDrive().getRightDistance()) ;
                    rb.addPlotData(plotid_, index_, 3, getTankDrive().getLeftVelocity()) ;
                    rb.addPlotData(plotid_, index_, 4, getTankDrive().getRightVelocity()) ;
                    rb.addPlotData(plotid_, index_, 5, getTankDrive().getAngle()) ;
                    rb.addPlotData(plotid_, index_, 6, lvoltage_) ;
                    rb.addPlotData(plotid_, index_, 7, rvoltage_) ;                    
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

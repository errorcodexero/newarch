#include "TankDriveScrubCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {

        TankDriveScrubCharAction::TankDriveScrubCharAction(TankDrive &drive, double power, bool highgear) : TankDriveAction(drive) {
            power_ = power ;
            high_gear_ = highgear ;
        }

        TankDriveScrubCharAction::~TankDriveScrubCharAction() {           
        }

        void TankDriveScrubCharAction::start() {
            is_done_ = false ;
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            start_angle_ = getTankDrive().getTotalAngle() ;
            start_left_ = getTankDrive().getLeftDistance() ;
            start_right_ = getTankDrive().getRightDistance() ;
            if (high_gear_)
                getTankDrive().highGear() ;
            else
                getTankDrive().lowGear() ;
                
            setMotorsToPercents(power_, -power_) ;
        }

        void TankDriveScrubCharAction::run() {
            if (!is_done_) {
                double angle = getTankDrive().getTotalAngle() - start_angle_ ;
                double distl = getTankDrive().getLeftDistance() - start_left_ ;
                double distr = getTankDrive().getRightDistance() - start_right_ ;

                if (angle > 3600.0)
                {
                    is_done_ = true ;
                    setMotorsToPercents(0.0, 0.0) ;

                    double avgc = (distl + distr) / 2.0 ;
                    double revs = angle / 360.0 ;
                    double effr = avgc / (xero::math::PI * revs) ;

                    std::cout << "Total Angle (NaVX) " << angle << std::endl ;
                    std::cout << "Left " << distl << std::endl ;
                    std::cout << "Right " << distr<< std::endl ;                        
                    std::cout << "Effective Width " << effr* 2.0 << std::endl ;
                }
                else
                {
                    std::cout << "Progress" ;
                    std::cout << " time " << frc::Timer::GetFPGATimestamp() - start_time_ ;
                    std::cout << " angle " << angle ;
                    std::cout << " left " << distl ;
                    std::cout << " right " << distr ;
                    std::cout << std::endl ;
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

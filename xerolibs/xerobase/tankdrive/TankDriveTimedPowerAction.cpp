#include "TankDriveTimedPowerAction.h"
#include "TankDrive.h"
#include "Robot.h"

namespace xero {
    namespace base {
        TankDriveTimedPowerAction::TankDriveTimedPowerAction(TankDrive &db, double left, double right, double time, bool lowgear) : TankDriveAction(db)  {
            left_ = left ;
            right_ = right ;
            duration_ = time ;
            lowgear_ = lowgear ;
        }

        TankDriveTimedPowerAction::TankDriveTimedPowerAction(TankDrive &db, const std::string &left, 
                const std::string &right, const std::string &time, bool lowgear) : TankDriveAction(db) {
            left_ = getTankDrive().getRobot().getSettingsParser().getDouble(left) ;
            right_ = getTankDrive().getRobot().getSettingsParser().getDouble(right) ;
            duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(time) ;            
            lowgear_ = lowgear ;
        }

        TankDriveTimedPowerAction::~TankDriveTimedPowerAction() {                
        }

        void TankDriveTimedPowerAction::start() {
            isdone_ = false ;
            start_ = getTankDrive().getRobot().getTime() ;
            if (getTankDrive().hasGearShifter()) {
                if (lowgear_)
                    getTankDrive().lowGear() ;
                else
                    getTankDrive().highGear() ;
            }
            std::cout << getTankDrive().getRobot().getTime() << " Set motors timed " << left_ << " " << right_ << std::endl ;
            setMotorsToPercents(left_, right_) ;
        }

        void TankDriveTimedPowerAction::run() {                
            isdone_ = (getTankDrive().getRobot().getTime() > start_ + duration_) ;
            if (isdone_) {
                setMotorsToPercents(0.0, 0.0) ;
                std::cout << getTankDrive().getRobot().getTime() << " stop motors" << std::endl ;
            }
        }

        bool TankDriveTimedPowerAction::isDone() {
            return isdone_ ;
        }

        void TankDriveTimedPowerAction::cancel()  {
            isdone_ = true ;
            setMotorsToPercents(0, 0) ;
        }

        std::string TankDriveTimedPowerAction::toString() {
            std::string ret ;

            ret = "TankDriveTimedPower: left " ;
            ret += std::to_string(left_) ;
            ret += ", right " ;
            ret += std::to_string(right_) ;
            ret += ", duration " ;
            ret += std::to_string(duration_) ;

            return ret ;
        }
    }
}
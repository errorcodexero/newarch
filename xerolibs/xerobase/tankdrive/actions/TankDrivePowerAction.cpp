#include "TankDrivePowerAction.h"
#include <tankdrive/TankDrive.h>
#include "Robot.h"

namespace xero {
    namespace base {
        TankDrivePowerAction::TankDrivePowerAction(TankDrive &db, double left, double right, bool highgear) : TankDriveAction(db)  {
            left_ = left ;
            right_ = right ;
            highgear_ = highgear ;
        }

        TankDrivePowerAction::TankDrivePowerAction(TankDrive &db, const std::string &left, const std::string &right, bool highgear) : TankDriveAction(db) {
            left_ = getTankDrive().getRobot().getSettingsParser().getDouble(left) ;
            right_ = getTankDrive().getRobot().getSettingsParser().getDouble(right) ;
            highgear_ = highgear ;            
        }

        TankDrivePowerAction::~TankDrivePowerAction() {                
        }

        void TankDrivePowerAction::start() {
            if (getTankDrive().hasGearShifter())
            {
                if (highgear_)
                    getTankDrive().highGear() ;
                else
                    getTankDrive().lowGear() ;
            }
            setMotorsToPercents(left_, right_) ;
        }

        void TankDrivePowerAction::run() {                
        }

        bool TankDrivePowerAction::isDone() {
            return true ;
        }

        void TankDrivePowerAction::cancel()  {
        }

        std::string TankDrivePowerAction::toString() {
            std::string ret ;

            ret = "TankDriveDutyCycle " ;
            ret += std::to_string(left_) ;
            ret += ", " ;
            ret += std::to_string(right_) ;

            return ret ;
        }
    }
}
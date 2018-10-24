#include "TankDrivePowerAction.h"
#include "TankDrive.h"
#include "Robot.h"

namespace xero {
    namespace base {
        TankDrivePowerAction::TankDrivePowerAction(TankDrive &db, double left, double right) : TankDriveAction(db)  {
            left_ = left ;
            right_ = right ;
        }

        TankDrivePowerAction::TankDrivePowerAction(TankDrive &db, const std::string &left, const std::string &right) : TankDriveAction(db) {
            left_ = getTankDrive().getRobot().getSettingsParser().getDouble(left) ;
            right_ = getTankDrive().getRobot().getSettingsParser().getDouble(right) ;
        }

        TankDrivePowerAction::~TankDrivePowerAction() {                
        }

        void TankDrivePowerAction::start() {
            getTankDrive().setMotorsToPercents(left_, right_) ;
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
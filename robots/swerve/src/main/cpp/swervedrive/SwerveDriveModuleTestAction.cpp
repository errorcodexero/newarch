#include "SwerveDriveModuleTestAction.h"
#include <frc/Timer.h>

namespace xero
{
    namespace base 
    {
        SwerveDriveModuleTestAction::SwerveDriveModuleTestAction(SwerveDriveBase &db, double dur, double rotate, double drive) : db_(db)
        {
            duration_ = dur ;
            rotate_power_ = rotate ;
            drive_power_ = drive ;
        }

        SwerveDriveModuleTestAction::~SwerveDriveModuleTestAction()
        {
        }

        void SwerveDriveModuleTestAction::start()
        {
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            running_ = true ;

            SwerveModule &mod = db_.getFL() ;
            mod.setRotatePower(rotate_power_) ;
            mod.setDrivePower(drive_power_) ;
        }

        void SwerveDriveModuleTestAction::run()
        {
            if (running_ && frc::Timer::GetFPGATimestamp() - start_time_ > duration_)
            {
                SwerveModule &mod = db_.getFL() ;
                mod.setRotatePower(0.0) ;
                mod.setDrivePower(0.0) ;
                running_ = false ;
            }
        }

        void SwerveDriveModuleTestAction::cancel()
        {

        }

        bool SwerveDriveModuleTestAction::isDone()
        {
            return !running_ ;
        }

        std::string SwerveDriveModuleTestAction::toString()
        {
            return std::string("SwerveDriveModuleTestAction") ;
        }
    }
}
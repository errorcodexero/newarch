#include "SwerveDriveModuleTestAction.h"
#include <iostream>
#include <frc/Timer.h>

namespace xero
{
    namespace base 
    {
        SwerveDriveModuleTestAction::SwerveDriveModuleTestAction(SwerveDriveBase &db, double dur, double rotate, double drive) : db_(db)
        {
            std::cout << "Creating SwerveDriveModuleTesetAction" << std::endl ;
            duration_ = dur ;
            rotate_power_ = rotate ;
            drive_power_ = drive ;
        }

        SwerveDriveModuleTestAction::~SwerveDriveModuleTestAction()
        {
        }

        void SwerveDriveModuleTestAction::start()
        {
            std::cout << "Starting SwerveDriveModuleTesetAction " << rotate_power_ << " " << drive_power_ << std::endl ;
            start_time_ = frc::Timer::GetFPGATimestamp() ;
            running_ = true ;

            SwerveModule &mod = db_.getFL() ;
            mod.setRotatePower(rotate_power_) ;
            mod.setDrivePower(drive_power_) ;
        }

        void SwerveDriveModuleTestAction::run()
        {
            double elapsed = frc::Timer::GetFPGATimestamp() - start_time_ ;
            if (running_)
            {
                if (elapsed > duration_)
                {
                    std::cout << "Finished SwerveDriveModuleTesetAction " << rotate_power_ << " " << drive_power_ << std::endl ; 
                    SwerveModule &mod = db_.getFL() ;
                    mod.setRotatePower(0.0) ;
                    mod.setDrivePower(0.0) ;
                    running_ = false ;
                }
                else
                {
                    std::cout << "Running SwerveDriveModuleTestAction " << elapsed << std::endl ;             
                }
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
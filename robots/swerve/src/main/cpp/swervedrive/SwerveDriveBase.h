#pragma once

#include "SwerveDriveBase.h"
#include "SwerveModule.h"
#include <DriveBase.h>
#include <list>
#include <iostream>

namespace xero
{
    namespace base
    {
        class SwerveDriveBase : public DriveBase
        {
        public:
            SwerveDriveBase(Robot& robot, int flr, int fld, int frr, int frd, int blr, int bld, int brr, int brd) ;
            virtual ~SwerveDriveBase() ;

            virtual double getVelocity() const {
                return 0.0 ;
            }

            virtual double getAngle() const {
                return 0.0 ;
            }

            virtual double getDist() const {
                return 0.0 ;
            }

            virtual bool canAcceptAction(ActionPtr action) {
                std::cout << "Accepting action" << std::endl ;
                return true ;
            }

            SwerveModule &getFL() { return *fl_ ; }
            SwerveModule &getFR() { return *fr_ ; }
            SwerveModule &getBL() { return *bl_ ; }
            SwerveModule &getBR() { return *br_ ; }                                    

        private:
            std::shared_ptr<SwerveModule> fl_ ;
            std::shared_ptr<SwerveModule> fr_ ;       
            std::shared_ptr<SwerveModule> bl_ ;
            std::shared_ptr<SwerveModule> br_ ;          
        } ;
    }
}
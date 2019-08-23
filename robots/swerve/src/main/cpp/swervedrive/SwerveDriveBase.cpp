#include "SwerveDriveBase.h"
#include <iostream>

namespace xero
{
    namespace base
    {
        SwerveDriveBase::SwerveDriveBase(Robot &robot, int flr, int fld, int frr, int frd, int blr, int bld, int brr, int brd) : DriveBase(robot, "Swerve")
        {
            std::cout << "SwerveDriveBase::SwerveDriveBase - creating drive base " << std::endl ;
            fl_ = std::make_shared<SwerveModule>(flr, fld) ;

#ifdef NOTYET
            fr_ = std::make_shared<SwerveModule>(frr, frd) ;
            bl_ = std::make_shared<SwerveModule>(blr, bld) ;
            br_ = std::make_shared<SwerveModule>(brr, brd) ;
#endif
        }

        SwerveDriveBase::~SwerveDriveBase()
        {
        }
    }
}
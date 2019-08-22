#include "SwerveDriveBase.h"

namespace xero
{
    namespace base
    {
        SwerveDriveBase::SwerveDriveBase(Robot &robot, int flr, int fld, int frr, int frd, int blr, int bld, int brr, int brd) : DriveBase(robot, "Swerve")
        {
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
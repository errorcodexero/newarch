#include "automodes/TestSwerveDriveModuleAutomode.h"
#include "SwerveRobot.h"
#include "swervedrive/SwerveDriveBase.h"
#include "swervedrive/SwerveDriveModuleTestAction.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {
        TestSwerveDriveModuleAutomode::TestSwerveDriveModuleAutomode(Robot &robot) : AutoMode(robot, "TestSwerveModule", "TestSwerveModule")
        {            
            auto &swerve = dynamic_cast<SwerveRobot &>(getRobot()) ;
            auto swerverobot = swerve.getSwerveRobotSubsystem() ;
            auto db = swerverobot->getSwerveDrive() ;

            pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 5, 0.5, 0.5)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 5.0, 0.2, -0.2)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 5.0, -0.2, 0.2)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 5.0, -0.2, -0.2)) ;

            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 60.0, 1.0, 1.0)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 60.0, 1.0, -1.0)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 60.0, -1.0, 1.0)) ;
            //pushSubActionPair(db, std::make_shared<SwerveDriveModuleTestAction>(*db, 60.0, -1.0, -1.0)) ;
        }

        TestSwerveDriveModuleAutomode::~TestSwerveDriveModuleAutomode()
        {            
        }
    }
}
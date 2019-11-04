#include "automodes/CenterHabTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>
#include <tankdrive/actions/TankDriveTimedPowerAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatch::CenterHabTwoHatch(Robot &robot, bool left, bool second, const char *name, const char *desc) : PhaserAutoModeBase(robot, name, desc)
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
      
            ActionPtr act ;
            
            //
            // Setup the camera for auto mode
            //
            act = std::make_shared<CameraChangeAction>(*vision, 0, CameraTracker::CameraMode::TargetTracking) ;
            pushSubActionPair(vision, act) ;

            //
            // Place first hatch
            //
            const char *angle = "turntable:angle:hatch:place:north" ;
            const char *height = "lifter:height:hatch:place:north:1" ;
            std::string path ;

#ifdef OLD_PATH_NAMES
            if (left)
                path = "CenterHab2CargoFrontLeft" ;
            else            
                path = "CenterHab2CargoFrontRight" ;
#else
            if (left)
                path = "CenterTwoLeftLS_CenterHab_LeftCargoShip" ;
            else            
                path = "CenterTwoRightLS_CenterHab_RightCargoShip" ;
#endif

            insertAutoModeLeg(height, angle, path, false, false, 0.0, 0.2, 60.0) ;

            if (second) {
                //
                // Collect the second hatch
                //

                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

#ifdef OLD_PATH_NAMES
                if (left)
                    path = "CargoFrontLeftLSLeft" ;
                else
                    path = "CargoFrontRightLSRight" ;
#else
                if (left)
                    path = "CenterTwoLeftLS_LeftCargoShip_LeftLS" ;
                else
                    path = "CenterTwoRightLS_RightCargoShip_RightLS" ;
#endif                    

                insertAutoModeLeg(height, angle, path, true, true, 2.0, 0.2, 60.0) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:1" ;
                std::string path ;

#ifdef OLD_PATH_NAMES
                if (left)
                    path = "LSLeftCargoFrontRight" ;
                else
                    path = "LSRightCargoFrontLeft" ;
#else
                if (left)
                    path = "CenterTwoLeftLS_LeftLS_RightCargoShip" ;
                else
                    path = "CenterTwoRightLS_RightLS_LeftCargoShip" ;
#endif

                insertAutoModeLeg(height, angle, path, false, false, 2.0, 0.2, 24.0) ;

                act = std::make_shared<TankDriveTimedPowerAction>(*db, 0.3, 0.3, 1.0) ;
                pushSubActionPair(db, act) ;
            }
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}
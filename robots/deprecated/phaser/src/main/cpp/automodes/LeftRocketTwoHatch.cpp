#include "automodes/LeftRocketTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>
#include <tankdrive/actions/TankDriveTimedPowerAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        LeftRocketTwoHatch::LeftRocketTwoHatch(Robot &robot, bool second) : PhaserAutoModeBase(robot, (second ? "TwoOnRocketLeft" : "OneOnRocketLeft"), "Rocket left auto mode") 
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto front = phaser.getPhaserRobotSubsystem()->getFrontLineSensor() ;
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
            path = "LeftHABLeftRocket" ;
#else
            path = "LeftHabLeftRocketTwo_LeftHab_LeftRocketFront" ;
#endif

            insertAutoModeLeg(height, angle, path, false, false, 0.0, 0.0, 48.0) ;

            if (second) {
                //
                // Collect the second hatch
                //
                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

#ifdef OLD_PATH_NAMES
                path = "LeftRocketLSLeft" ;
#else
                path = "LeftHabLeftRocketTwo_LeftRocketFront_LeftLS" ;
#endif

                insertAutoModeLeg(height, angle, path, true, true, 2.0, 0.2, 60.0) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:2" ;
                std::string path ;

#ifdef OLD_PATH_NAMES
                path = "LSLeftRocketLeft" ;
#else
                path = "LeftHabLeftRocketTwo_LeftLS_LeftRocketFront" ;
#endif

                insertAutoModeLeg(height, angle, path, false, false, 2.0, 0.2, 48.0) ;
            }
        }

        LeftRocketTwoHatch::~LeftRocketTwoHatch()
        {            
        }    
    }
}
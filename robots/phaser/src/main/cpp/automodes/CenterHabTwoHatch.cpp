#include "automodes/CenterHabTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatch::CenterHabTwoHatch(Robot &robot, bool left, bool second, const char *name, const char *desc) : PhaserAutoModeBase(robot, name, desc)
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
      
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

            if (left)
                path = "CenterHab2CargoFrontRight" ;
            else            
                path = "CenterHab2CargoFrontLeft" ;

            insertAutoModeLeg(height, angle, path, false, false, 0.0, 0.2, 60.0) ;

            if (second) {
                //
                // Collect the second hatch
                //

                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

                if (left)
                    path = "CargoFrontRightLSLeft" ;
                else
                    path = "CargoFrontLeftLSRight" ;

                insertAutoModeLeg(height, angle, path, true, true, 2.0, 0.2, 48.0) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:1" ;
                std::string path ;

                if (left)
                    path = "LSLeftCargoFrontLeft" ;
                else
                    path = "LSRightCargoFrontRight" ;

                insertAutoModeLeg(height, angle, path, false, false, 2.0, 0.2, 36.0) ;
            }
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}
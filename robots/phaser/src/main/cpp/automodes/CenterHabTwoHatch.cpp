#include "automodes/CenterHabTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>
#include "gamepiecemanipulator/ScoreHatch.h"
#include "gamepiecemanipulator/CompleteLSHatchCollect.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatch::CenterHabTwoHatch(Robot &robot, bool left, bool second) : PhaserAutoModeBase(robot, "CenterHabTwoHatch", "Start in center, score two on cargo ship, getting extra hatch from left loading station")
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
                path = "CenterHab2CargoFrontLeft" ;
            else
                path = "CenterHab2CargoFrontRight" ;

            act = std::make_shared<ScoreHatch>(*game) ;            
            insertAutoModeLeg(height, angle, path, false, act) ;

            if (second) {
                //
                // Collect the second hatch
                //

                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

                if (left)
                    path = "CargoFrontLeftLSLeft" ;
                else
                    path = "CargoFrontLeftLSRight" ;

                act = std::make_shared<CompleteLSHatchCollect>(*game) ;
                insertAutoModeLeg(height, angle, path, true, act) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:1" ;
                std::string path ;

                if (left)
                    path = "LSLeftCargoFrontRight" ;
                else
                    path = "LSRightCargoFrontLeft" ;

                act = std::make_shared<ScoreHatch>(*game) ;            
                insertAutoModeLeg(height, angle, path, false, act) ;
            }
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}
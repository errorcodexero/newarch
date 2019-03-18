#include "automodes/RightRocketTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>
#include "gamepiecemanipulator/ScoreHatch.h"
#include "gamepiecemanipulator/CompleteLSHatchCollect.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        RightRocketTwoHatch::RightRocketTwoHatch(Robot &robot, bool second) : PhaserAutoModeBase(robot, "RightRocketTwoHatch", (second ? "Start on right hab level, score two on rocket" : "start on right hab level, score one on rocket"))
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

            path = "RightHABRightRocket" ;

            act = std::make_shared<ScoreHatch>(*game) ;            
            insertAutoModeLeg(height, angle, path, false, 0.0, act) ;

            if (second) {
                //
                // Collect the second hatch
                //

                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

                path = "RightRocketLSRight" ;

                act = std::make_shared<CompleteLSHatchCollect>(*game) ;
                insertAutoModeLeg(height, angle, path, true, 2.0, act) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:2" ;
                std::string path ;

                path = "LSRightRocketRight" ;

                act = std::make_shared<ScoreHatch>(*game) ;            
                insertAutoModeLeg(height, angle, path, false, 2.0, act) ;
            }
        }

        RightRocketTwoHatch::~RightRocketTwoHatch()
        {            
        }    
    }
}
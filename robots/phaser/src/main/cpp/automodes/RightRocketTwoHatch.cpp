#include "automodes/RightRocketTwoHatch.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>

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

#ifdef OLD_PATH_NAMES
            path = "RightHABRightRocket" ;
#else
            path = "RightHabRightRocketTwo_RightHab_RightRocketFront" ;
#endif

            insertAutoModeLeg(height, angle, path, false, true, 0.0, 0.0, 36.0) ;
            if (second) {
                //
                // Collect the second hatch
                //

                angle = "turntable:angle:hatch:collect:south" ;
                height = "lifter:height:hatch:collect:south" ;

#ifdef OLD_PATH_NAMES
                path = "RightRocketLSRight" ;
#else
                path = "RightHabRightRocketTwo_RightRocketFront_RightLS" ;
#endif

                insertAutoModeLeg(height, angle, path, true, false, 2.0, 0.2, 36.0) ;

                //
                // Place the second hatch
                //
                const char *angle = "turntable:angle:hatch:place:north" ;
                const char *height = "lifter:height:hatch:place:north:2" ;
                std::string path ;
                
#ifdef OLD_PATH_NAMES
                path = "LSRightRocketRight" ;
#else
                path = "RightHabRightRocketTwo_RightLS_RightRocketFront" ;
#endif

                insertAutoModeLeg(height, angle, path, false, true, 2.0, 0.2, 18.0) ;
            }
        }

        RightRocketTwoHatch::~RightRocketTwoHatch()
        {            
        }    
    }
}
#include "RanseurRobotTurtleAction.h"
#include "RanseurSubsystemAction.h"
#include "RanseurRobotSubsystem.h"
#include "RanseurRobotTurtleAction.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
#include "tubmanipulatorsubsystem/TubManipulatorSubsystem.h"
#include "bunnyarm/BunnyArm.h"
#include "bunnyarm/BunnyArmDeployAction.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <actions/Action.h>
#include "Subsystem.h"

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace ranseur {

        std::string RanseurRobotTurtleAction::action_name("RanseurRobotTurtleAction");

        RanseurRobotTurtleAction::RanseurRobotTurtleAction(RanseurRobotSubsystem &robotss) : RanseurSubsystemAction(robotss) {
            double v ;                                          //value
            ActionPtr act ;                                     //action
            auto arm = robotss.getTubManipulatorSubsystem()->getTubArm() ;
            auto wrist = robotss.getTubManipulatorSubsystem()->getTubWrist() ;
            auto bunny_arm = robotss.getBunnyArm() ;

            ///Arm/// 
            v = robotss.getRobot().getSettingsParser().getDouble("tubarm:turtle:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            parallel_.addSubActionPair(arm, act) ;

            ///Wrist///
            v = robotss.getRobot().getSettingsParser().getDouble("tubwrist:turtle:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            parallel_.addSubActionPair(wrist, act) ;

            // Bunny Arm //
            act = std::make_shared<BunnyArmDeployAction>(*bunny_arm, true) ;
            parallel_.addSubActionPair(bunny_arm, act) ;

            ///   ^^^ grab the angle values from the dat files ^^^   ///
            ///add the action and the subsytem to the parallel action///

        }

        RanseurRobotTurtleAction::~RanseurRobotTurtleAction() {            
        }
        
        void RanseurRobotTurtleAction::start() {
            parallel_.start() ;
        }

        void RanseurRobotTurtleAction::run() {
            parallel_.run() ;
        }

        bool RanseurRobotTurtleAction::isDone() {
            return parallel_.isDone(); 
        }

        void RanseurRobotTurtleAction::cancel() {
            parallel_.cancel() ;
        }

    }
}

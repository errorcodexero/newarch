#include "PhoenixTestTuneMode.h"
#include "Phoenix.h"
#include "liftingcollector/LiftingCollector.h"
#include "phlifter/LifterBreakAction.h"
#include "phlifter/LifterShiftAction.h"
#include <actions/Action.h>
#include <tankdrive/actions/TankDriveCharAction.h>
#include <motorencodersubsystem/MotorEncoderPowerAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::base;
using namespace xero::misc;

namespace xero
{
    namespace phoenix
    {
        PhoenixTestTuneMode::PhoenixTestTuneMode(xero::base::Robot &robot)
            : PhoenixAutoMode(robot, "TestMode", "Runs various tests based on the params file")
        {
            ActionPtr act ;
            auto &phoenix = dynamic_cast<Phoenix &>(getRobot()) ;
            auto tankdrive = phoenix.getPhoenixRobotSubsystem()->getTankDrive() ;
            auto lift = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter();

            int mode = robot.getSettingsParser().getInteger("auto:testmode:which");
            double dist = robot.getSettingsParser().getDouble("auto:testmode:distance");
            double power = robot.getSettingsParser().getDouble("auto:testmode:power");
            double duration = robot.getSettingsParser().getDouble("auto:testmode:duration");

            switch(mode)
            {
            case 0:     // Drive base characterization
                pushSubActionPair(tankdrive, std::make_shared<TankDriveCharAction>(*tankdrive, duration, power, true)) ;
                break;

            case 1:     // Lift characterization
                pushSubActionPair(lift, std::make_shared<LifterShiftAction>(*lift, true)) ;                          // Shift to high gear (make this false for low gear)
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, false)) ;                         // Disable the break                
                pushSubActionPair(lift, std::make_shared<MotorEncoderPowerAction>(*lift, power, duration)) ;         // Run the lift motor
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, true)) ;                          // Enable the break                 
                break ;

            case 2:     // Lift goto test
                pushSubActionPair(lift, std::make_shared<LifterShiftAction>(*lift, true)) ;                          // Shift to high gear (make this false for low gear)
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, false)) ;                         // Disable the break               
                pushSubActionPair(lift, std::make_shared<MotorEncoderGoToAction>(*lift, dist));
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, true)) ;                          // Enable the break                 
                break;
            }
        }

        PhoenixTestTuneMode::~PhoenixTestTuneMode()
        {
        }
    }
}
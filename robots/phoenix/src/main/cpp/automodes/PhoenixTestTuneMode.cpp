#include "PhoenixTestTuneMode.h"
#include "Phoenix.h"
#include "liftingcollector/LiftingCollector.h"
#include "phlifter/LifterBreakAction.h"
#include "phlifter/LifterShiftAction.h"
#include "collector/Collector.h"
#include "grabber/Grabber.h"
#include <actions/Action.h>
#include <actions/DelayAction.h>
#include <tankdrive/actions/TankDriveCharAction.h>
#include <tankdrive/actions/TankDriveScrubCharAction.h>
#include <tankdrive/actions/TankDriveAngleAction.h>
#include <motorencodersubsystem/MotorEncoderPowerAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>

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
            auto grabber = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getGrabber();
            auto intake = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getIntake();

            int mode = robot.getSettingsParser().getInteger("auto:testmode:which");
            double dist = robot.getSettingsParser().getDouble("auto:testmode:distance");
            double power = robot.getSettingsParser().getDouble("auto:testmode:power");
            double duration = robot.getSettingsParser().getDouble("auto:testmode:duration");

            switch(mode)
            {
            case 0:     // Drive base straight characterization
                pushSubActionPair(tankdrive, std::make_shared<TankDriveCharAction>(*tankdrive, duration, power, true)) ;
                break;

            case 1:     // Drive base rotate characterization (note duration is total angle)
                pushSubActionPair(tankdrive, std::make_shared<TankDriveScrubCharAction>(*tankdrive, power, duration, true));
                break;

            case 2:
                pushSubActionPair(tankdrive, std::make_shared<TankDriveAngleAction>(*tankdrive, dist));
                break;

            case 10:     // Lift characterization
                pushSubActionPair(lift, std::make_shared<LifterShiftAction>(*lift, true)) ;                          // Shift to high gear (make this false for low gear)
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, false)) ;                         // Disable the break                
                pushSubActionPair(lift, std::make_shared<MotorEncoderPowerAction>(*lift, power, duration)) ;         // Run the lift motor
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, true)) ;                          // Enable the break                 
                break ;

            case 11:     // Lift goto test
                pushSubActionPair(lift, std::make_shared<LifterShiftAction>(*lift, true)) ;                          // Shift to high gear (make this false for low gear)
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, false)) ;                         // Disable the break               
                pushSubActionPair(lift, std::make_shared<MotorEncoderGoToAction>(*lift, dist));
                pushSubActionPair(lift, std::make_shared<LifterBreakAction>(*lift, true)) ;                          // Enable the break                 
                break;

            case 20:    // Grabber characterization
                pushSubActionPair(grabber, std::make_shared<MotorEncoderPowerAction>(*grabber, power, duration)) ;   // Run the lift motor            
                break;

            case 21:    // Test goto angle
                pushSubActionPair(grabber, std::make_shared<MotorEncoderGoToAction>(*grabber, 90));                 // Go to a fixed angle
                pushAction(std::make_shared<DelayAction>(phoenix.getMessageLogger(), 2.0));
                pushSubActionPair(grabber, std::make_shared<MotorEncoderGoToAction>(*grabber, 0));                 // Go to a fixed angle                
                break;                

            case 30:    // Intake tests
                pushSubActionPair(intake, std::make_shared<SingleMotorPowerAction>(*intake, power, duration)) ;     // Run the lift motor            
                break;
            }
        }

        PhoenixTestTuneMode::~PhoenixTestTuneMode()
        {
        }
    }
}
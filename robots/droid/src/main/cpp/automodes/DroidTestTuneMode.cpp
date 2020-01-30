#include "DroidTestTuneMode.h"
#include "Droid.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/intake/Intake.h"
#include "gamepiecemanipulator/conveyor/Conveyor.h"
#include "gamepiecemanipulator/intake/CollectAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"
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
    namespace droid
    {
        DroidTestTuneMode::DroidTestTuneMode(xero::base::Robot &robot)
            : DroidAutoMode(robot, "TestMode", "Runs various tests based on the params file")
        {
            ActionPtr act ;
            auto &droid = dynamic_cast<Droid &>(getRobot()) ;
            auto tankdrive = droid.getDroidSubsystem()->getTankDrive() ;
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            int mode = robot.getSettingsParser().getInteger("auto:testmode:which");
            double dist = robot.getSettingsParser().getDouble("auto:testmode:distance");
            double power = robot.getSettingsParser().getDouble("auto:testmode:power");
            double duration = robot.getSettingsParser().getDouble("auto:testmode:duration");

            (void)dist ;

            switch(mode)
            {
            case 0:     // Drive base straight characterization
                pushSubActionPair(tankdrive, std::make_shared<TankDriveCharAction>(*tankdrive, duration, power, true)) ;
                break;

            case 1:     // Drive base rotate characterization (note duration is total angle)
                pushSubActionPair(tankdrive, std::make_shared<TankDriveScrubCharAction>(*tankdrive, power, duration, true));
                break;

            case 2:
                pushSubActionPair(intake, std::make_shared<CollectAction>(*intake, true)) ;
                pushSubActionPair(conveyor, std::make_shared<ConveyorEmitAction>(*conveyor)) ;
                break ;

            case 3:
                pushSubActionPair(intake, std::make_shared<CollectAction>(*intake, true)) ;
                break ;

            }
        }

        DroidTestTuneMode::~DroidTestTuneMode()
        {
        }
    }
}
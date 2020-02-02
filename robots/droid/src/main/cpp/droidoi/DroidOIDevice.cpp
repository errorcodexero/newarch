#include "DroidOIDevice.h"
#include "DroidOISubsystem.h"
#include "Droid.h"
#include "turret/FollowTargetAction.h"
#include "turret/Turret.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"
#include "gamepiecemanipulator/ShootAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {

        DroidOIDevice::DroidOIDevice(DroidOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ;
        }

        DroidOIDevice::~DroidOIDevice() {
        }

        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void DroidOIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            // Automode - ten position switch
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;

            // Shoot/Collect mode - toggle switch
            size_t collect_shoot_mode_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:shoot_collect_mode") ;            
            collect_shoot_mode_ = mapButton(collect_shoot_mode_b, OIButton::ButtonType::Level) ;       

            // Shoot on/off - toggle switch
            size_t shoot_on_off_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:shoot_onoff") ;            
            shoot_on_off_ = mapButton(shoot_on_off_b, OIButton::ButtonType::Level) ;

            // Collect - push button
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collect_onoff") ;
            collect_on_off_ = mapButton(collect_b, OIButton::ButtonType::LowToHigh) ;
        }

        void DroidOIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto game = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto conveyor = game->getConveyor() ;            

            //
            // First check the mode
            //
            if (getValue(collect_shoot_mode_) == 1 && mode_ != RobotMode::Shooting)
            {
                //
                // TODO: We need to be sure we are not in the middle of collecting a ball
                //
                if (!game->isActive())
                {
                    turret->setAction(follow_target_) ;
                    conveyor->setAction(prepare_to_emit_) ;
                    mode_ = RobotMode::Shooting ;
                }
            }
            else if (getValue(collect_shoot_mode_) == 0 && mode_ != RobotMode::Collecting)
            {
                //
                // TODO: We need to be sure we are not in the middle of shooting a ball
                //
                if (!game->isActive())
                {
                    turret->setAction(nullptr) ;
                    conveyor->setAction(prepare_to_receive_) ;
                    mode_ = RobotMode::Collecting ;
                }
            }

            //
            // Now based on the mode, see if we need to start or stop the
            // current action
            //
            if (mode_ == RobotMode::Collecting)
            {
                if (getValue(collect_on_off_) == 1 && !collecting_ && !game->isActive())
                {
                    game->setAction(start_collect_);
                    collecting_ = true;
                }
                else if (getValue(collect_on_off_) == 0 && collecting_ && !game->isActive())
                {
                    game->setAction(stop_collect_);
                    collecting_ = false;
                }
            }
            else if (mode_ == RobotMode::Shooting)
            {
                if (getValue(shoot_on_off_) == 1 && !shooting_)
                {
                    game->setAction(shoot_);
                    shooting_ = true;
                }
                else if (getValue(shoot_on_off_) == 0 && shooting_ && !game->isActive())
                {
                    game->setAction(nullptr);
                    shooting_ = false;
                }                
            }
        }

        //
        // Create static actions we for the OI
        //
        void DroidOIDevice::init() {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto db = droid.getDroidSubsystem()->getTankDrive() ; 
            auto game = droid.getDroidSubsystem()->getGamePieceManipulator() ;                       
            auto conveyor = game->getConveyor() ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            follow_target_ = std::make_shared<FollowTargetAction>(*turret) ;
            prepare_to_emit_ = std::make_shared<ConveyorPrepareToEmitAction>(*conveyor) ;
            prepare_to_receive_ = std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor) ;
            start_collect_ = std::make_shared<StartCollectAction>(*game);
            stop_collect_ = std::make_shared<StopCollectAction>(*game);
            shoot_ = std::make_shared<ShootAction>(*game);
        }
    }
}

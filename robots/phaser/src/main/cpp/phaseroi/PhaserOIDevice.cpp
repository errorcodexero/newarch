#include "PhaserOIDevice.h"
#include "PhaserOISubsystem.h"
#include "Phaser.h"
#include "carloshatch/DumpHatch.h"
#include <cameratracker/CameraChangeAction.h>
#include <ParallelAction.h>
#include "carloshatch/CarlosHatchArmAction.h"
#include "gamepiecemanipulator/GamePieceAction.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include "gamepiecemanipulator/CompleteLSCargoCollect.h"
#include "gamepiecemanipulator/ScoreCargo.h"
#include "gamepiecemanipulator/ReadyAction.h"
#include "gamepiecemanipulator/ResetIntakesAction.h"
#include "gamepiecemanipulator/CalibrateManip.h"
#include "cargointake/CargoIntakeAction.h"
#include "carloshatch/CarlosHatchImpactAction.h"
#include "singlemotorsubsystem/SingleMotorPowerAction.h"
#include "phasercameratracker/DriveByVisionAction.h"
#include "lifter/LifterPowerAction.h"
#include "lifter/LifterGoToHeightAction.h"
#include "lightsensor/LineFollowAction.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include "phaserrobotsubsystem/ClimbAction.h"
#include "phaserrobotsubsystem/StrafeAction.h"
#include "climber/ClimberDeployAction.h"
#include <oi/DriverGamepadRumbleAction.h>
#include "LineFollowerTakeover.h"
#include "VisionDetectTakeover.h"
#include "phaserids.h"
#include <Robot.h>
#include <lightsensor/LightSensorSubsystem.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <TerminateAction.h>
#include <DelayAction.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        PhaserOIDevice::PhaserOIDevice(PhaserOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ;

            //
            // Initialize state maintained by the OI
            //
            height_ = ActionHeight::LevelOne ;
            dir_ = Direction::North ;
            mode_ = OperationMode::Invalid ;
            ship_cargo_state_ = RocketShipMode::Invalid ;

            last_angle_requested_ = "@" ;
            last_height_requested_ = "@" ;

            field_relative_ = false ;
            const char *fr = "oi:fieldrelative" ;
            if (sub.getRobot().getSettingsParser().isDefined(fr))
                field_relative_ = sub.getRobot().getSettingsParser().getBoolean(fr) ;

            piece_ = GamePieceType::Invalid ;
            prev_cargo_hatch_switch_ = -1 ;

            extra_arm_button_ = sub.getRobot().getSettingsParser().getBoolean("oi:extra_arm_button") ;
        }

        PhaserOIDevice::~PhaserOIDevice() {
        }

        std::string PhaserOIDevice::toString(OperationMode mode) {
            std::string ret = "?????" ;

            switch(mode) {
                case OperationMode::Auto:
                    ret = "Auto" ;
                    break ;
                case OperationMode::Invalid:
                    ret = "Invalid" ;
                    break ;
                case OperationMode::Manual:
                    ret = "Manual" ;
                    break ;
                case OperationMode::SemiAuto:
                    ret = "SemiAuto" ;
                    break ;
            }

            return ret ;
        }

        std::string PhaserOIDevice::toString(Direction dir) {
            std::string ret = "????" ;

            switch(dir) {
            case Direction::North:
                ret = "north" ;
                break ;

            case Direction::South:
                ret = "south" ;
                break ;

            case Direction::East:
                ret = "east" ;
                break ;

            case Direction::West:
                ret = "west" ;
                break ;
            }

            return ret ;
        }

        std::string PhaserOIDevice::toString(ActionHeight h) {
            std::string ret = "????" ;

            switch(h) {
            case ActionHeight::LevelOne:
                ret = "1" ;
                break ;

            case ActionHeight::LevelTwo:
                ret = "2" ;
                break ;

            case ActionHeight::LevelThree:
                ret = "3" ;
                break ;

            case ActionHeight::CargoBay:
                ret = "c" ;
                break ;
            }

            return ret ;
        }


        //
        // Read the mode switch.  If it differs from the current mode, call UpdateMode()
        // to update the current mode.
        //
        void PhaserOIDevice::getTrackingMode() {
            OperationMode mode = OperationMode::Auto ;
            int value = getValue(tracking_manual_switch_) ;

            switch(value) {
            case 0:
                mode = OperationMode::Auto ;
                break ;
            case 1:
                mode = OperationMode::SemiAuto ;
                break ;
            case 2:
                mode = OperationMode::Manual ;
                break ;
            }
            if (mode != mode_)
                updateMode(mode) ;
        }

        //
        // Update the mode of the robot.  This should changes the camera mode and assocaited LED as necessary
        //
        void PhaserOIDevice::updateMode(OperationMode newmode) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            if ((mode_ == OperationMode::Manual || mode_ == OperationMode::SemiAuto || mode_ == OperationMode::Invalid) && newmode == OperationMode::Auto)
            {
                // Switch camera to tracking mode
                auto act = std::make_shared<CameraChangeAction>(*camera, camera->getCameraIndex(), CameraTracker::CameraMode::TargetTracking) ;
                camera->setAction(act) ;
            }
            else if (mode_ == OperationMode::Auto && (newmode == OperationMode::Manual || newmode == OperationMode::SemiAuto || mode_ == OperationMode::Invalid))
            {
                // Switch camera to viewing mode
                auto act = std::make_shared<CameraChangeAction>(*camera, camera->getCameraIndex(), CameraTracker::CameraMode::DriverViewing) ;
                camera->setAction(act) ;
            }

            std::string newmodestr = toString(newmode) ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: updating mode" ;
            log << ", old mode " << toString(mode_) ;
            log << ", new mode " << newmodestr ; ;
            log.endMessage() ;

            mode_ = newmode ;
            frc::SmartDashboard::PutString("RobotMode", newmodestr) ;
        }

        //
        // Get the cargo/hatch mode based on any game piece that is in the robot as well
        // as the cargo/hatch switch
        //
        void PhaserOIDevice::getCargoHatchMode(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            size_t camerano = HatchCamera ;

            piece_ = game->getGamePieceType() ;

            if (piece_ == GamePieceType::Cargo) {
                activity_ = ActivityType::Place ;
                camerano = CargoCamera ;
                frc::SmartDashboard::PutBoolean("PlaceCollect", true) ;
                frc::SmartDashboard::PutBoolean("CargoHatch", true) ;
            }
            else if (piece_ == GamePieceType::Hatch) {
                activity_ = ActivityType::Place ;
                camerano = HatchCamera ;
                frc::SmartDashboard::PutBoolean("PlaceCollect", true) ;
                frc::SmartDashboard::PutBoolean("CargoHatch", false) ;
            }
            else {
                activity_ = ActivityType::Collect ;
                int v = getValue(hatch_cargo_switch_) ;
                if (v != prev_cargo_hatch_switch_) {
                    prev_cargo_hatch_switch_ = v ;
                    height_ = ActionHeight::LevelOne ;
                }
                if (v) {
                    camerano = CargoCamera ;
                    frc::SmartDashboard::PutBoolean("PlaceCollect", false) ;
                    frc::SmartDashboard::PutBoolean("CargoHatch", true) ;
                    piece_ = GamePieceType::Cargo ;
                }
                else {
                    camerano = HatchCamera ;
                    frc::SmartDashboard::PutBoolean("PlaceCollect", false) ;
                    frc::SmartDashboard::PutBoolean("CargoHatch", false) ;
                    piece_ = GamePieceType::Hatch ;
                }
            }

            if (camera->getCameraIndex() != camerano) {
                MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
                log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
                log << "OI: getCargoHatchMode - changing camera to " ;
                if (camerano == HatchCamera)
                    log << "hatch" ;
                else
                    log << "cargo" ;
                log.endMessage() ;

                ActionPtr ptr = std::make_shared<CameraChangeAction>(*camera, camerano, camera->getCameraMode()) ;
                seq.pushSubActionPair(camera, ptr) ;
            }
        }

        //
        // Get the value of the rocker versus cargo ship switch.
        //
        void PhaserOIDevice::getShipRocketMode()
        {
            RocketShipMode rst ;
            if (getValue(ship_rocket_)) {
                rst = RocketShipMode::CargoShip ;
            }
            else {
                rst = RocketShipMode::Rocket ;
            }

            if (rst != ship_cargo_state_) {
                if (ship_cargo_state_ == RocketShipMode::CargoShip && height_ == ActionHeight::LevelThree) {
                    //
                    // We are switching from cargo ship to rocket.  The height may be at level three when strafing
                    // the third line on the ship.  Reset this to level two if this is the case so we don't suddenly
                    // jump to level three just by changing this switch and hitting a direction.
                    //
                    height_ = ActionHeight::LevelTwo ;
                }

                ship_cargo_state_ = rst ;
            }
        }

        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void PhaserOIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;
            int button ;
            SettingsParser &settings = getSubsystem().getRobot().getSettingsParser() ;

            button = settings.getInteger("oi:hatch_cargo_switch") ;
            hatch_cargo_switch_ = mapButton(button, OIButton::ButtonType::LevelInv) ;

            button = settings.getInteger("oi:tracking_viewing_nothing_switch") ;
            tracking_manual_switch_ = mapAxisSwitch(button, 3) ;

            button = settings.getInteger("oi:climb_lock_switch") ;
            climb_lock_switch_ = mapButton(button, OIButton::ButtonType::Level) ;

            button = settings.getInteger("oi:compass_north") ;
            compass_north_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:compass_south") ;
            compass_south_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:compass_east") ;
            compass_east_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:compass_west") ;
            compass_west_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:height_level_one") ;
            height_level_one_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:height_level_two") ;
            height_level_two_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:height_level_three") ;
            height_level_three_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:height_cargo_bay") ;
            height_cargo_bay_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:collect_floor") ;
            collect_floor_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:go") ;
            go_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:turtle") ;
            turtle_mode_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:climb") ;
            climb_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:calibrate") ;
            calibrate_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:reverse") ;
            reverse_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:ship_rocket") ;
            ship_rocket_ = mapAxisSwitch(button, 3) ;
        }

        //
        // Create static actions we for the OI
        //
        void PhaserOIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto hatch_holder = game->getHatchHolder() ;
            auto climber = ph.getPhaserRobotSubsystem()->getClimber() ;
            auto turntable = game->getTurntable() ;
            auto vision = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            safe_height_ = getSubsystem().getRobot().getSettingsParser().getDouble("turntable:safe_lifter_height") ;

            finish_collect_hatch_ = std::make_shared<CarlosHatchImpactAction>(*game->getHatchHolder()) ;
            finish_place_hatch_ = std::make_shared<CarlosHatchImpactAction>(*game->getHatchHolder()) ;
            finish_collect_cargo_ = std::make_shared<CompleteLSCargoCollect>(*game) ;
            finish_place_cargo_  = std::make_shared<ScoreCargo>(*game) ;

            set_collect_cargo_floor_  = std::make_shared<FloorCollectCargoAction>(*game) ;
            reset_intakes_ = std::make_shared<ResetIntakeAction>(*game) ;
            climb_action_ = std::make_shared<ClimbAction>(*ph.getPhaserRobotSubsystem(), true) ;

            deploy_climber_ = std::make_shared<ClimberDeployAction>(*climber) ;

            calibrate_action_ = std::make_shared<CalibrateManip>(*game) ;

            retract_arm_ = std::make_shared<CarlosHatchArmAction>(*game->getHatchHolder(), CarlosHatchArmAction::Operation::RETRACT) ;
        }

        PhaserOIDevice::Direction PhaserOIDevice::getRobotDirection() {
            Direction ret = Direction::North ;
            double angle = getSubsystem().getRobot().getDriveBase()->getAngle() ;

            if (angle <= 45.0 && angle >= -45.0)
                ret = Direction::North ;
            else if (angle <= 135.0 && angle > 45.0)
                ret = Direction::West ;
            else if (angle > 135.0 || angle < -135.0)
                ret = Direction::South ;
            else if (angle < -45.0 && angle >= -135.0)
                ret = Direction::East ;

            return ret ;
        }

        PhaserOIDevice::Direction PhaserOIDevice::compassToFieldRelative(PhaserOIDevice::Direction compass)
        {
            static Direction fieldmapping[4][4] = {
                { Direction::North, Direction::South, Direction::East, Direction::West },            // Robot direction N
                { Direction::South, Direction::North, Direction::West, Direction::East },            // Robot direction S
                { Direction::East, Direction::West, Direction::South, Direction::North },            // Robot direction E
                { Direction::West, Direction::East, Direction::North, Direction::South },            // Robot direction W
            } ;

            Direction robotdir = getRobotDirection() ;
            int rdir = static_cast<int>(robotdir) ;
            int cdir = static_cast<int>(compass) ;
            return fieldmapping[rdir][cdir] ;
        }

        //
        // See if a direction is being pressed on the joystick and if so, store the
        // direction and return true.
        //
        bool PhaserOIDevice::getDirection() {
            bool ret = false ;
            Direction compass = Direction::North ;
            double angle = getSubsystem().getRobot().getDriveBase()->getAngle() ;

            if (getValue(compass_north_)) {
                compass = Direction::North ;
                ret = true ;
            }
            else if (getValue(compass_south_)) {
                compass = Direction::South ;
                ret = true ;
            }
            else if (getValue(compass_east_)) {
                compass = Direction::East ;
                ret = true ;
            }
            else if (getValue(compass_west_)) {
                compass = Direction::West ;
                ret = true ;
            }

            if (ret) {
                if (field_relative_)
                    compass = compassToFieldRelative(compass) ;
                dir_ = compass ;

                MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
                log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
                log << "OI:" ;
                log << " angle " << angle ;
                log << " stick " << toString(compass) ;
                log << " field " << toString(dir_) ;
                log.endMessage() ;
            }

            return ret ;
        }

        //
        // See if a height button is being pressed on the OI.  If so, store the height
        // and return true.
        //
        bool PhaserOIDevice::getHeightButton() {
            bool ret = false ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;

            if (getValue(height_level_one_)) {
                height_ = ActionHeight::LevelOne ;
                ret = true ;
                log << "OI: Level 1" ;
            }
            else if (getValue(height_level_two_)) {
                height_ = ActionHeight::LevelTwo ;
                ret = true ;
                log << "OI: Level 2" ;
            }
            else if (getValue(height_level_three_)) {
                height_ = ActionHeight::LevelThree ;
                ret = true ;
                log << "OI: Level 3" ;
            }
            else if (getValue(height_cargo_bay_)) {
                height_ = ActionHeight::CargoBay ;
                ret = true ;
                log << "OI: Cargo Height" ;
            }
            log.endMessage() ;
            return ret ;
        }

        void PhaserOIDevice::generateDirectionActionsCollectHatch(std::string &height, std::string &angle) {
            //
            // Loading hatch from the loading station
            //
            height = "lifter:height:hatch:collect:" ;
            height += toString(dir_)  ;

            angle = "turntable:angle:hatch:collect:" ;
            angle += toString(dir_) ;
        }

        void PhaserOIDevice::generateDirectionActionsCollectCargo(std::string &height, std::string &angle) {
                //
                // Loading cargo from the loading station
                //
                height = "lifter:height:cargo:collect:" ;
                height += toString(dir_) ;

                angle = "turntable:angle:cargo:collect:" ;
                angle += toString(dir_) ;
        }

        void PhaserOIDevice::generateDirectionActionsPlaceHatch(std::string &height, std::string &angle) {
            height = "lifter:height:hatch:place:" ;
            height += toString(dir_) + ":1" ;
            angle = "turntable:angle:hatch:place:" ;
            angle += toString(dir_) ;
        }

        void PhaserOIDevice::generateDirectionActionsPlaceCargo(std::string &height, std::string &angle) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;

            if (mode_ == OperationMode::Auto) {
                //
                // The roboti sholding cargo and we are in auto mode, set the
                // height so that the camera can see the vision targets.
                //
                height = "lifter:height:cargo:place:" ;
                height += toString(dir_) + ":" ;

                if (getValue(ship_rocket_))
                    height += "c" ;
                else
                    height += toString(height_) ;

                angle = "turntable:angle:cargo:place:" ;
                angle += toString(dir_) ;
            }
            else {
                //
                // The robot is holding cargo, the action will be to place the cargo
                // at the requested height
                //
                if (game->getLifter()->getHeight() >= safe_height_) {
                    height = "@" ;
                } else {
                    height = "lifter:height:cargo:place:" ;
                    height += toString(dir_) + ":1" ;
                }
                angle = "turntable:angle:cargo:place:" ;
                angle += toString(dir_) ;
            }
        }

        //
        // Generate the actions associated with a direction button on the joystick.  The
        // generated action raises the lift to a safe height, rotates the turntable to the
        // desired angle based on the action to be performed, and then sets the lift at a
        // level one height to ready for the operation.
        //
        void PhaserOIDevice::generateDirectionActions(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            ActionPtr act, finish ;
            std::string height, angle ;

            MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            logger << "generateDirectionActions: " ;
            logger << toString(piece_) ;
            logger << " " ;
            logger << toString(activity_) ;
            logger.endMessage() ;

            auto hh = ph.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;

            if (piece_ == GamePieceType::Cargo && hh->isArmDeployed())
                seq.pushSubActionPair(hh, retract_arm_) ;            

            if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Place)
                generateDirectionActionsPlaceHatch(height, angle) ;
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Place)
                generateDirectionActionsPlaceCargo(height, angle)  ;
            else if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Collect)
                generateDirectionActionsCollectHatch(height, angle) ;
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Collect)
                generateDirectionActionsCollectCargo(height, angle) ;

            gotoHeightAngle(seq, height, angle) ;
        }

        void PhaserOIDevice::gotoHeightAngle(SequenceAction &seq, const std::string &height, const std::string &angle, bool leave) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            ActionPtr act ;

            last_angle_requested_ = angle ;
            last_height_requested_ = height ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating direction related actions: " ;
            log << "height " << height ;
            log << ", angle " << angle ;
            log.endMessage() ;

            SettingsParser &parser = getSubsystem().getRobot().getSettingsParser() ;
            if ((height == "@" || parser.isDefined(height)) && (angle == "@" || parser.isDefined(angle))) {
                act = std::make_shared<ReadyAction>(*game, height, angle, leave) ;
                seq.pushSubActionPair(game, act, false) ;
            }
            else {
                MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
                if (!parser.isDefined(height)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Missing expected param file value '" ;
                    logger << height ;
                    logger << "'" ;
                    logger.endMessage() ;
                }

                if (!parser.isDefined(angle)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Missing expected param file value '" ;
                    logger << angle ;
                    logger << "'" ;
                    logger.endMessage() ;
                }
            }
        }

        void PhaserOIDevice::generateHeightActionsPlaceHatch(std::string &height, std::string &angle)
        {
            //
            // The robot is holding a hatch, the action will be to place a hatch
            // at the requested height
            //
            height = "lifter:height:hatch:place:" ;
            height += toString(dir_) + ":" ;
            height += toString(height_) ;

            angle = last_angle_requested_ ;
        }

        void PhaserOIDevice::generateHeightActionsPlaceCargo(std::string &height, std::string &angle)
        {
            //
            // The robot is holding cargo, the action will be to place the cargo
            // at the requested height
            //
            height = "lifter:height:cargo:place:" ;
            height += toString(dir_) + ":" ;
            if (ship_cargo_state_ == RocketShipMode::CargoShip && mode_ == OperationMode::Auto) {
                //
                // If we are in auto strafe mode, and we are in cargo ship mode, we always go to
                // cargo ship height as the level 1, level 2, level 3 switches set which line to
                // hit
                //
                height += "c" ;
            }
            else {
                height += toString(height_) ;
            }

            angle = last_angle_requested_ ;
        }

        void PhaserOIDevice::generateHeightActionsCollectCargo(std::string &height, std::string &angle)
        {
            //
            // Loading hatch from the loading station
            //
            height = "lifter:height:cargo:collect:" ;
            height += toString(dir_) ;

            angle = last_angle_requested_ ;
        }

        void PhaserOIDevice::generateHeightActionsCollectHatch(std::string &height, std::string &angle)
        {
            //
            // Loading hatch from the loading station
            //
            height = "lifter:height:hatch:collect:" ;
            height += toString(dir_)  ;

            angle = last_angle_requested_ ;
        }

        void PhaserOIDevice::generateHeightActions(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            std::string height, angle ;
            bool leave = false ;

            MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            logger << "generateHeightActions: " ;
            logger << toString(piece_) ;
            logger << " " ;
            logger << toString(activity_) ;
            logger.endMessage() ;

            if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Place) {
                generateHeightActionsPlaceHatch(height, angle) ;
                leave = true ;
            }
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Place) 
                generateHeightActionsPlaceCargo(height, angle) ;
            else if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Collect) {
                generateHeightActionsCollectHatch(height, angle) ;
                leave = true ;
            }
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Collect)
                generateHeightActionsCollectCargo(height, angle) ;

            gotoHeightAngle(seq, height, angle, leave) ;

        }

        void PhaserOIDevice::generateArmActionsHatchVisionLine(SequenceAction &seq)
        {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto hatchholder = ph.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;

            if (dir_ == Direction::East || dir_ == Direction::West) {
                //
                // Trying to do auto hatch placement based on the side
                //
                MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Hatch placement on the side of the robot not supported" ;
                logger.endMessage() ;
            }
            else {
                //
                // Doing north or south
                //
                bool push = true ;

                if (height_ == ActionHeight::LevelTwo || height_ == ActionHeight::LevelThree)
                    push = false ;

                //
                // Always clear out any existing detectors in the teleop controller
                //
                auto ctrl = ph.getCurrentController() ;
                std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
                teleop->clearDetectors() ;

                ActionPtr act = std::make_shared<CarlosHatchImpactAction>(*hatchholder, push) ;
                seq.pushSubActionPair(hatchholder, act) ;

                setupVisionDetectors() ;
            }
        }

        void PhaserOIDevice::generateArmActionsHatchLine(SequenceAction &seq)
        {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto hatchholder = ph.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;

            if (dir_ == Direction::East || dir_ == Direction::West) {
                //
                // Trying to do auto hatch placement based on the side, not supported
                //
                MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Hatch placement on the side of the robot not supported" ;
                logger.endMessage() ;
            }
            else {
                //
                // Doing north or south
                //
                bool push = true ;

                if (height_ == ActionHeight::LevelTwo || height_ == ActionHeight::LevelThree)
                    push = false ;                

                //
                // Always clear out any existing detectors in the teleop controller
                //
                auto ctrl = ph.getCurrentController() ;
                std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
                teleop->clearDetectors() ;

                ActionPtr act = std::make_shared<CarlosHatchImpactAction>(*hatchholder, push) ;
                seq.pushSubActionPair(hatchholder, act) ;

                setupLineFollowingDetectors() ;
            }
        }

        void PhaserOIDevice::generateArmActionsHatchManual(SequenceAction &seq)
        {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto hatchholder = ph.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;
            bool push = true ;
            if (height_ == ActionHeight::LevelTwo || height_ == ActionHeight::LevelThree)
                push = false ;
            ActionPtr act = std::make_shared<CarlosHatchImpactAction>(*hatchholder, push) ;
            seq.pushSubActionPair(hatchholder, act) ;
        }

        void PhaserOIDevice::generateArmActionsCargoVisionLine(SequenceAction &seq)
        {
            if (dir_ == Direction::East || dir_ == Direction::West)
                setupStrafe(seq) ;
            else {
                setupVisionDetectors() ;
            }
        }

        void PhaserOIDevice::generateArmActionsCargoLine(SequenceAction &seq)
        {
            if (dir_ == Direction::East || dir_ == Direction::West)
                setupStrafe(seq) ;
            else {
                setupLineFollowingDetectors() ;
            }
        }

        void PhaserOIDevice::generateArmActionsCargoManual(SequenceAction &seq)
        {
            // NOP
        }

        void PhaserOIDevice::generateArmActions(SequenceAction &seq)
        {
            if (piece_ == GamePieceType::Hatch && mode_ == OperationMode::Auto)
                generateArmActionsHatchVisionLine(seq) ;
            else if (piece_ == GamePieceType::Hatch && mode_ == OperationMode::SemiAuto)
                generateArmActionsHatchLine(seq) ;
            else if (piece_ == GamePieceType::Hatch && mode_ == OperationMode::Manual)
                generateArmActionsHatchManual(seq) ;
            else if (piece_ == GamePieceType::Cargo && mode_ == OperationMode::Auto)
                generateArmActionsCargoVisionLine(seq) ;
            else if (piece_ == GamePieceType::Cargo && mode_ == OperationMode::SemiAuto)
                generateArmActionsCargoLine(seq) ;
            else if (piece_ == GamePieceType::Cargo && mode_ == OperationMode::Manual)
                generateArmActionsCargoManual(seq) ;
        }

        void PhaserOIDevice::setupStrafe(SequenceAction &seq)
        {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            ph.getPhaserRobotSubsystem()->setAction(nullptr) ;

            strafe_ = nullptr ;
            if (getValue(ship_rocket_)) {
                //
                // Cargo ship, the height buttons really tell us which line
                //
                switch(height_) {
                case ActionHeight::CargoBay:
                    break ;
                case ActionHeight::LevelOne:
                    strafe_ = std::make_shared<StrafeAction>(*ph.getPhaserRobotSubsystem(), 1) ;
                    break ;
                case ActionHeight::LevelTwo:
                    strafe_ = std::make_shared<StrafeAction>(*ph.getPhaserRobotSubsystem(), 2) ;
                    break ;
                case ActionHeight::LevelThree:
                    strafe_ = std::make_shared<StrafeAction>(*ph.getPhaserRobotSubsystem(), 3) ;
                    break ;
                }
                ph.getPhaserRobotSubsystem()->setAction(strafe_) ;
            }
            else {
                //
                // Rocket ship, always do first line
                //
                strafe_ = std::make_shared<StrafeAction>(*ph.getPhaserRobotSubsystem()) ;
                ph.getPhaserRobotSubsystem()->setAction(strafe_) ;
            }
        }

        //
        // Setup detector that will take over if the vision detects a target that meets the
        // takeover critera.  For now, this assumes that the vision target will cause the
        // line followers to take over.  We may want to enhance this in the future so that if the
        // DriveByVision finished, we execute the finish action.
        //
        void PhaserOIDevice::setupVisionDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            MessageLogger &log = ph.getMessageLogger() ;
            auto oi = ph.getOI() ;
            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto lifter = game->getLifter() ;
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
            std::shared_ptr<ParallelAction> parallel = std::make_shared<ParallelAction>() ;

            //
            // Setup the threshold for vision
            //
            camera->setVisionDetectThreshold(60.0) ;

            std::shared_ptr<LightSensorSubsystem> linefollower ;
            ActionPtr drive , lineaction ;

            //
            //   Sequence seq 
            //     TerminateAction (term=linefollower)
            //       DriveByVision
            //     FollowLineAction
            //     FinishAction (if cargo)
            //     RumbleAction
            //

            if (dir_ == Direction::North) {
                drive = std::make_shared<DriveByVisionAction>(*db, *camera, false) ;
                linefollower = ph.getPhaserRobotSubsystem()->getFrontLineSensor() ;
                lineaction = std::make_shared<LineFollowAction>(*linefollower, *db, "linefollower:front:power", "linefollower:front:distance", "linefollower:front:adjust") ;
            }
            else if (dir_ == Direction::South) {
                drive = std::make_shared<DriveByVisionAction>(*db, *camera, true) ;
                linefollower = ph.getPhaserRobotSubsystem()->getBackLineSensor() ;
                lineaction = std::make_shared<LineFollowAction>(*linefollower, *db, "linefollower:back:power", "linefollower:back:distance", "linefollower:back:adjust") ;
            }

            SequenceActionPtr seq = std::make_shared<SequenceAction>(log) ;

            //
            // Push the rumble to indicate we took over
            //
            ActionPtr rumble = std::make_shared<DriverGamepadRumbleAction>(*oi, false, 0.25, 0.5) ;
            seq->pushSubActionPair(oi, rumble) ;

            //
            // Create the drive by vision action, it is terminated by detecting a line on the right line follower
            //
            std::shared_ptr<TerminateAction> term = std::make_shared<TerminateAction>(db, drive, getSubsystem().getRobot()) ;
            term->addTerminator(linefollower) ;

            // First action in sequence, drive by vision
            seq->pushAction(term) ;

            // Second action in sequence, drive by line follower
            seq->pushSubActionPair(db, lineaction) ;

            // Third action in sequence, finish the action (if cargo)
            if (piece_ == GamePieceType::Cargo)
                seq->pushSubActionPair(game, getFinishAction()) ;

            parallel->addAction(seq) ;            
            teleop->addDetector(std::make_shared<VisionDetectTakeover>(teleop, parallel, *camera)) ;

            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: setup vision detector" ;
            log.endMessage() ;
            teleop->printDetectors() ;
        }

        //
        // Setup detector that will take over if the line follower detects a line.
        //
        void PhaserOIDevice::setupLineFollowingDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto lifter = game->getLifter() ;
            auto oi = ph.getOI() ;
            ActionPtr finish = getFinishAction() ;
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
            MessageLogger &log = ph.getMessageLogger() ;
            SequenceActionPtr seq = std::make_shared<SequenceAction>(log) ;
            std::shared_ptr<LightSensorSubsystem> line ;
            std::shared_ptr<LineFollowAction> drive ;

            if (dir_ == Direction::North) {
                line = ph.getPhaserRobotSubsystem()->getFrontLineSensor() ;
                drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:front:power", "linefollower:front:distance", "linefollower:front:adjust") ;
            }
            else if (dir_ == Direction::South) {
                line = ph.getPhaserRobotSubsystem()->getBackLineSensor() ;
                drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:back:power", "linefollower:back:distance", "linefollower:back:adjust") ;
            }

            ActionPtr rumble = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 0.25, 0.5) ;

            seq->pushSubActionPair(db, drive) ;
            if (piece_ == GamePieceType::Cargo)
                seq->pushSubActionPair(game, getFinishAction()) ;
                
            seq->pushSubActionPair(oi, rumble) ;

            teleop->addDetector(std::make_shared<LineFollowerTakeover>(teleop, seq, *line)) ;

            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: setup line detector" ;
            log.endMessage() ;
            teleop->printDetectors() ;
        }

        //
        // Called when a height button is pressed
        //
        void PhaserOIDevice::generateHeightButtonActions(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto hh = ph.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;

            if (piece_ == GamePieceType::Cargo && hh->isArmDeployed())
                seq.pushSubActionPair(hh, retract_arm_) ;

            //
            // Generate actions to move to the right height and angle
            //
            generateHeightActions(seq) ;

            if (piece_ == GamePieceType::Cargo || !extra_arm_button_)
            {
                //
                // Arm the robot to complete the action
                //
                generateArmActions(seq) ;
            }
        }

        //
        // Get the finish action for the current mode.  The finish action assumes the turntable
        // and the lifter are at the right place.
        //
        xero::base::ActionPtr PhaserOIDevice::getFinishAction() {
            ActionPtr ret = nullptr ;;
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;

            if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Collect) {
                ret = finish_collect_hatch_ ;
            }
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Collect) {
                ret = finish_collect_cargo_ ;
            }
            else if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Place) {
                ret = finish_place_hatch_ ;
            }
            else if (piece_ == GamePieceType::Cargo && activity_ == ActivityType::Place) {
                ret = finish_place_cargo_ ;
            }

            return ret ;
        }

        //
        // Manually finish an action if a detector has not taken over and
        // finished the action.
        //
        void PhaserOIDevice::manuallyFinish(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
            ActionPtr act = getFinishAction() ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: manually finishing action:" ;
            log << act->toString() ;
            log.endMessage() ;

            teleop->clearDetectors() ;
            if (piece_ == GamePieceType::Cargo)
                seq.pushSubActionPair(game, act) ;
            else
                seq.pushSubActionPair(game->getHatchHolder(), act) ;
        }

        bool PhaserOIDevice::isIdleOrReadyAction() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;

            if (game->isDone())
                return true ;

            auto rdyact = std::dynamic_pointer_cast<ReadyAction>(game->getAction()) ;
            return rdyact != nullptr ;
        }

        void PhaserOIDevice::generateActions(SequenceAction &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto hatch_holder = game->getHatchHolder() ;
            auto fcol = std::dynamic_pointer_cast<FloorCollectCargoAction>(set_collect_cargo_floor_) ;

            //
            // Get the tracking mode, either auto, semi-auto, or manual
            //
            getTrackingMode() ;

            //
            // Get the cargo/hatch mode
            //
            getCargoHatchMode(seq) ;

            //
            // Check the rocket vs cargo ship switch
            //
            getShipRocketMode() ;

            //
            // Calibrate takes precedence over everything
            //
            if (getValue(calibrate_)) {
                if (calibrate_action_->isDone())
                    std::cout << "calibrate action still running" << std::endl ;;
                if (reset_intakes_->isDone())
                    std::cout << "reset intakes still running" << std::endl ;
                if (climb_action_->isDone())
                    std::cout << "climb_action still running" << std::endl ;
                    
                game->setAction(calibrate_action_) ;
            }
            else if (getValue(turtle_mode_) && calibrate_action_->isDone() && reset_intakes_->isDone() && climb_action_->isDone()) {
                //
                // Directly assign the action and make it forcing to ensure this
                // action takes priority over everything else.
                //
                game->setAction(reset_intakes_, true) ;

            } else if (game->isDone() && getValue(climb_lock_switch_) && getValue(climb_)) {
                //
                // Climb lock switch is off and the climb button is pushed.
                //
                // Time to fly .... like a grasshopper
                //
                auto robotsub = ph.getPhaserRobotSubsystem() ;
                robotsub->setAction(climb_action_) ;
            }            
            else if (!set_collect_cargo_floor_->isDone() && getValue(reverse_)) {
                //
                // Reverse the direction of the intake rollers, but only if we are
                // doing a floor collect
                //
                fcol->reverseIntake() ;
            }
            else if (piece_ == GamePieceType::Hatch && activity_ == ActivityType::Place && getValue(reverse_)) {
                //
                // Dump a hatch if we are setup to place one.  This can also be used to 
                // place a hatch at level three.
                //
                auto act = std::make_shared<DumpHatch>(*hatch_holder) ;
                seq.pushSubActionPair(hatch_holder, act) ;
            }
            else if (!finish_collect_cargo_->isDone() && getValue(go_)) {
                //
                // We are collecting cargo from the loading station and go we
                // hit again, cancel the action
                //
                game->cancelAction() ;
            }
            else if (!set_collect_cargo_floor_->isDone() && getValue(collect_floor_)) {
                //
                // We are floor collecting caro and the floor collect buttons was hit
                // again.  Cancel the operation
                //
                game->cancelAction() ;

            }
            else if (isIdleOrReadyAction() && getDirection()) {
                //
                // The joystick has been pressed in a direction, move the
                // lift and turntable to match to desired target.  Note this only work if
                // the gamepiecemanipulator is finished with any previous action, or the
                // action in progress is a direction action.
                //
                generateDirectionActions(seq) ;
            }
            else if (isIdleOrReadyAction() && getHeightButton()) {
                generateHeightButtonActions(seq) ;
            }
            else if (game->isDone() && getValue(go_)) {
                //
                // Manually initiate the seleced operation
                //
                manuallyFinish(seq) ;
            }
            else if (game->isDone() && getValue(collect_floor_)) {
                if (game->getGamePieceType()  == GamePieceType::None) {
                    //
                    // Collect game pieces from the floor
                    //
                    game->setAction(set_collect_cargo_floor_) ;
                }
            }
        }
    }
}

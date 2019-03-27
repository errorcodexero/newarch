#pragma once

#include "gamepiece.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include <oi/OIDevice.h> 
#include <Action.h>
#include <ActionSequence.h>

namespace xero {
    namespace phaser {
        class PhaserOISubsystem ;

        class PhaserOIDevice : public xero::base::OIDevice {
        public:
            enum class Direction
            {
                North = 0,
                South = 1,
                East = 2,
                West = 3,
            } ;

            enum class ActionHeight
            {
                LevelOne = 1,
                LevelTwo = 2,
                LevelThree = 3,
                CargoBay = 4,
            } ;

            enum class OperationMode
            {
                Auto,
                SemiAuto,
                Manual,
                Invalid
            } ;

            enum class RocketShipMode
            {
                Rocket,
                CargoShip,
                Invalid,
            } ;

            enum class ActivityType
            {
                Collect,
                Place,
            } ;

        public:
            PhaserOIDevice(PhaserOISubsystem &sub, int index) ;
            virtual ~PhaserOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::ActionSequence &seq) ;

            virtual void init() ;

        private:
            void bindOI() ;
            void getTrackingMode() ;
            void getCargoHatchMode(xero::base::ActionSequence &seq) ;
            bool getDirection() ;
            bool getHeightButton() ;
            void getShipRocketMode() ;

            Direction getRobotDirection() ;
            Direction compassToFieldRelative(Direction dir) ;

            void gotoHeightAngle(xero::base::ActionSequence &seq, const std::string &height, const std::string &angle) ;

            void updateMode(OperationMode mode) ;

            void generateDirectionActions(xero::base::ActionSequence &seq) ;
            void generateDirectionActionsCollectHatch(std::string &height, std::string &angle) ;
            void generateDirectionActionsCollectCargo(std::string &height, std::string &angle) ;
            void generateDirectionActionsPlaceHatch(std::string &height, std::string &angle) ;
            void generateDirectionActionsPlaceCargo(std::string &height, std::string &angle) ;

            void generateHeightActions(xero::base::ActionSequence &seq) ;
            void generateHeightActionsCollectCargo(std::string &height, std::string &angle) ;
            void generateHeightActionsCollectHatch(std::string &height, std::string &angle) ;
            void generateHeightActionsPlaceCargo(std::string &height, std::string &angle) ;
            void generateHeightActionsPlaceHatch(std::string &height, std::string &angle) ;

            void generateArmActions(xero::base::ActionSequence &seq) ;            
            void generateArmActionsHatchVisionLine(xero::base::ActionSequence &seq) ;     
            void generateArmActionsHatchLine(xero::base::ActionSequence &seq) ;  
            void generateArmActionsHatchManual(xero::base::ActionSequence &seq) ;  
            void generateArmActionsCargoVisionLine(xero::base::ActionSequence &seq) ;
            void generateArmActionsCargoLine(xero::base::ActionSequence &seq) ;
            void generateArmActionsCargoManual(xero::base::ActionSequence &seq) ;   

            void generateHeightButtonActions(xero::base::ActionSequence &seq) ;

            void setupStrafe(xero::base::ActionSequence &seq) ;
            void setupVisionDetectors() ;
            void setupLineFollowingDetectors() ;

            void manuallyFinish(xero::base::ActionSequence &seq) ;

            xero::base::ActionPtr getFinishAction() ;

            std::string toString(Direction dir) ;
            std::string toString(ActionHeight height) ;
            std::string toString(OperationMode mode) ;

            std::string generateActionHeightName(bool tracking) ;

            bool isIdleOrReadyAction() ;

            std::string toString(ActivityType a) {
                std::string str ;

                switch(a) {
                case ActivityType::Collect:
                    str = "Collect" ;
                    break ;
                case ActivityType::Place:
                    str = "Place" ;
                    break ;
                }

                return str ;
            }

            std::string toString(GamePieceType g) {
                std::string str ;

                switch(g) {
                case GamePieceType::Cargo:
                    str = "Cargo" ;
                    break ;

                case GamePieceType::Hatch:
                    str = "Hatch" ;
                    break ;

                case GamePieceType::Invalid:
                    str = "Invalid" ;
                    break ;

                case GamePieceType::None:
                    str = "None" ;
                    break ;
                }

                return str;
            }

        private:
            static const size_t HatchCamera = 0 ;
            static const size_t CargoCamera = 1 ;
            
        private:
            size_t automode_ ;

            size_t hatch_cargo_switch_ ;
            size_t tracking_manual_switch_ ;

            // joystick for turret 
            size_t compass_north_ ;
            size_t compass_south_ ;
            size_t compass_east_ ;
            size_t compass_west_ ;

            // \height takes input from gamepiece holders 
            size_t height_level_one_ ;
            size_t height_level_two_ ;
            size_t height_level_three_ ;
            size_t height_cargo_bay_ ;

            // \collect takes input from hatch_cargo_switch_ 
            // \when holders empty, height_level_one_ will set lift to loading station height
            size_t collect_floor_ ;
            size_t go_ ; 
            size_t turtle_mode_ ;

            size_t climb_lock_switch_ ;
            size_t climb_ ;

            size_t calibrate_ ;
            size_t reverse_ ; 

            size_t ship_rocket_ ;

            Direction dir_ ;
            ActionHeight height_ ;
            ActivityType activity_ ;
            OperationMode mode_ ;
            RocketShipMode ship_cargo_state_ ;

            xero::base::ActionPtr set_collect_cargo_floor_ ;

            xero::base::ActionPtr reset_intakes_ ;
            xero::base::ActionPtr extend_finger_ ;
            xero::base::ActionPtr retract_finger_ ;

            xero::base::ActionPtr direction_action_ ;

            xero::base::ActionPtr finish_collect_hatch_ ;
            xero::base::ActionPtr finish_collect_cargo_ ;
            xero::base::ActionPtr finish_place_hatch_ ;
            xero::base::ActionPtr finish_place_cargo_ ;    

            xero::base::ActionPtr deploy_climber_ ;
            xero::base::ActionPtr climb_action_ ; 

            xero::base::ActionPtr calibrate_action_ ;
            
            xero::base::ActionPtr track_cargo_target_ ;

            xero::base::ActionPtr strafe_ ;
            
            double safe_height_ ;

            std::string last_angle_requested_ ;
            std::string last_height_requested_ ;

            bool field_relative_ ;

            GamePieceType piece_ ;
        } ;
    }
}
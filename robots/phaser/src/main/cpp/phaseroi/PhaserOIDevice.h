#pragma once

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include <oi/OIDevice.h> 
#include <Action.h>

namespace xero {
    namespace phaser {
        class PhaserOISubsystem ;

        class PhaserOIDevice : public xero::base::OIDevice {
        public:
            enum class Direction
            {
                North,
                South,
                East,
                West,
            } ;

            enum class ActionHeight
            {
                LevelOne,
                LevelTwo,
                LevelThree,
                CargoBay,
            } ;

            enum class OperationMode
            {
                Auto,
                SemiAuto,
                Manual,
                Invalid
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

            void updateMode(OperationMode mode) ;
            void generateDirectionActions(xero::base::ActionSequence &seq) ;
            void generateHeightButtonActions(xero::base::ActionSequence &seq) ;
            void generateTargetHeightActions(xero::base::ActionSequence &seq) ;

            void setupVisionDetectors() ;
            void setupLineFollowingDetectors() ;

            void manuallyFinish(xero::base::ActionSequence &seq) ;

            xero::base::ActionPtr getFinishAction() ;

            std::string dirToString() ;
            std::string heightToString() ;
            std::string toString(OperationMode mode) ;

            std::string generateActionHeightName() ;

        private:
            static const size_t HatchCamera = 0 ;
            static const size_t CargoCamera = 1 ;
            
        private:
            double hatch_finger_start_ ;
            bool has_hatch_state_ ;
            double hatch_finger_delay_ ;

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

            Direction dir_ ;
            ActionHeight height_ ;
            OperationMode mode_ ;

            xero::base::ActionPtr set_collect_hatch_floor_ ;
            xero::base::ActionPtr set_collect_cargo_floor_ ;

            xero::base::ActionPtr reset_intakes_ ;
            xero::base::ActionPtr extend_finger_ ;
            xero::base::ActionPtr retract_finger_ ;

            xero::base::ActionPtr finish_collect_hatch_ ;
            xero::base::ActionPtr finish_collect_cargo_ ;
            xero::base::ActionPtr finish_place_hatch_ ;
            xero::base::ActionPtr finish_place_cargo_ ;    

            xero::base::ActionPtr deploy_climber_ ;
            xero::base::ActionPtr climb_action_ ; 

            xero::base::ActionPtr calibrate_action_ ;
            
            double safe_height_ ;
        } ;
    }
}
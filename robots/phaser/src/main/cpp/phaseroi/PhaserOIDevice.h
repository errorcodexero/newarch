#pragma once

#include <OIDevice.h> 
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

        public:
            PhaserOIDevice(PhaserOISubsystem &sub, int index) ;
            virtual ~PhaserOIDevice() ;

            virtual int getAutoModeSelector() {
                return getValue(automode_) ;
            }

            virtual void generateActions(xero::base::ActionSequence &seq) ;

        private:
            void createActions() ;
            void initialize() ;
            bool getDirection() ;
            bool getHeight() ;
            
            void generateReadyAction() ;

            std::string dirToString() ;
            std::string heightToString() ;
            
        private:
            size_t automode_ ;
            size_t camera_switch_ ;
            size_t camera_mode_ ;   

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
            size_t collect_loading_station_ ; 

            // \ score takes input from gamepiece holders
            size_t score_ ;

            size_t climb_lock_switch_ ;
            size_t climb_ ;

            size_t extra_button_ ; 

            Direction dir_ ;
            ActionHeight height_ ;

            xero::base::ActionPtr set_collect_hatch_floor_ ;
            xero::base::ActionPtr set_collect_cargo_floor_ ;
        } ;
    }
}
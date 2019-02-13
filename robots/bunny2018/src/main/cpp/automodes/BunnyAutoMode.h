#pragma once

#include <AutoController.h>
#include <Action.h>
#include <AutoMode.h>

namespace xero {
    namespace bunny2018 {
        class ShooterEjectAutoBallAction ;
        
        /// \brief This is the base class for any autonomous mode controller
        /// This class contains the code to execute an automode program and
        /// the derived classs generally creates the auto program for a given
        /// robot, automode selector, and set of field conditions.
        class BunnyAutoMode : public xero::base::AutoController
        {
        public:
            BunnyAutoMode(xero::base::Robot &robot) ;

            virtual ~BunnyAutoMode() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::AutoModePtr createDriveStraightChar() ;
            xero::base::AutoModePtr createDriveStraightTest() ;
            xero::base::AutoModePtr createGameAutoMode(int number_of_crates) ;
            xero::base::AutoModePtr createTestMotors() ;
            xero::base::AutoModePtr createShooterTestMode() ;

            void addCrateLocations(std::shared_ptr<ShooterEjectAutoBallAction> act, int count) ;
        };
    }
}

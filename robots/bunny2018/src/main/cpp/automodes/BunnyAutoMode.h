#pragma once

#include <AutoController.h>
#include <Action.h>
#include <ActionSequence.h>

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
            xero::base::ActionSequencePtr createDriveStraightChar() ;
            xero::base::ActionSequencePtr createDriveStraightTest() ;
            xero::base::ActionSequencePtr createGameAutoMode(int number_of_crates) ;
            xero::base::ActionSequencePtr createTestMotors() ;
            xero::base::ActionSequencePtr createShooterTestMode() ;

            void addCrateLocations(std::shared_ptr<ShooterEjectAutoBallAction> act, int count) ;
        };
    }
}

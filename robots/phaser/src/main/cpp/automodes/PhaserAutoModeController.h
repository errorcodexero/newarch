#pragma once

#include <AutoController.h>
#include <AutoMode.h>
#include <string>

namespace xero {
    namespace phaser {
        class PhaserAutoModeController : public xero::base::AutoController {
        public:
            PhaserAutoModeController(xero::base::Robot &robot) ;
            virtual ~PhaserAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::AutoModePtr createStraightCharAutoMode() ;
            xero::base::AutoModePtr createScrubCharAutoMode() ;
            xero::base::AutoModePtr createTestOne() ;
            xero::base::AutoModePtr createTestTwo() ;
            xero::base::AutoModePtr testHatchDeposit() ;
            xero::base::AutoModePtr testLineFollower() ;
            xero::base::AutoModePtr testHatchHolder() ;
            xero::base::AutoModePtr testLifter() ;        
            xero::base::AutoModePtr testVision() ;   
            xero::base::AutoModePtr testTurntable() ; 

            xero::base::AutoModePtr testCargoHolder() ;
            xero::base::AutoModePtr testCargoIntake() ;
            xero::base::AutoModePtr testClimber() ;

            xero::base::AutoModePtr testFloorCollectCargo() ;

            xero::base::ActionPtr lifterGoToHeight(double height) ;
            xero::base::ActionPtr turntableGoToAngle(double angle) ;
            xero::base::ActionPtr driveAndPosition(const std::string &path, double height, double angle) ;
        } ;
    }
}

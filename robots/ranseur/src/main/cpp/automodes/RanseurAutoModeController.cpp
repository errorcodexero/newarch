#include "automodes/RanseurAutoModeController.h"
#include "automodes/AutoModeXero.h"
#include "automodes/AutoModeTest.h"
#include "Ranseur.h"

#include <MessageLogger.h>
#include <tankdrive/modes/StraightCharAutoMode.h>
#include <tankdrive/modes/FollowPathAutomode.h>
#include <frc/DriverStation.h>

using namespace xero::base ;
using namespace xero::misc ;

std::vector<bool> reverses = { false, true } ;

namespace xero {
    namespace ranseur {
        RanseurAutoModeController::RanseurAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        RanseurAutoModeController::~RanseurAutoModeController() {
        }

        void RanseurAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                mode = std::make_shared<AutoModeXero>(getRobot()) ;
                break ;

            case 1:
                mode = std::make_shared<AutoModeTest>(getRobot()) ;
                break ;                
                
            case 2:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.6, 3.0) ;
                break ;

            case 3:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "Test_TestPath") ;
                break ;
            }
            setAction(mode) ;
        }
    }
}

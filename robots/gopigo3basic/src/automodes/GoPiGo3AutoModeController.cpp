#include "GoPiGo3AutoModeController.h"
#include <tankdrive/FollowPathAutomode.h>
#include <tankdrive/TankDriveScrubMode.h>
#include <tankdrive/StraightCharAutoMode.h>

using namespace xero::base ;

namespace xero {
    namespace gopigo3 {
        GoPiGo3AutoModeController::GoPiGo3AutoModeController(Robot &robot) : AutoController(robot) {

        }

        GoPiGo3AutoModeController::~GoPiGo3AutoModeController() {           
        }

        void GoPiGo3AutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr ptr = nullptr ;
            switch(sel)
            {
            case 0:
                break ;
            case 1:
                break ;
            case 2:
                break ;
            case 3:
                break ;
            case 4:
                break ;
            case 5:
                break ;
            case 6:
                break;
            case 7:
                break ;
            case 8:
                break ;
            case 9:
                ptr = std::make_shared<StraightCharAutoMode>(getRobot(), 0.5, 5.0) ;               
                break ;
            }
            setAction(ptr) ;
        }
    }
}

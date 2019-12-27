#include "PhoenixAutoController.h"
#include "ThreeCubeAutoMode.h"
#include <tankdrive/modes/StraightCharAutoMode.h>
#include <tankdrive/modes/FollowPathAutomode.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixAutoController::PhoenixAutoController(Robot &robot) : AutoController(robot) {
        }
        
        PhoenixAutoController::~PhoenixAutoController() {
        }

        void PhoenixAutoController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                mode = std::make_shared<ThreeCubeAutoMode>(getRobot(), ThreeCubeAutoMode::Side::Right);
                break ;

            case 1:
                mode = std::make_shared<ThreeCubeAutoMode>(getRobot(), ThreeCubeAutoMode::Side::Left);            
                break ;                
                
            case 2:
                break ;

            case 9:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.5, 3.0, true) ;
                break ;
            }
            setAction(mode) ;
        }
    }
}

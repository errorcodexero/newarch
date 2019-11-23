#include "automodes/RanseurAutoModeController.h"
#include "automodes/AutoModeXero.h"
#include "Ranseur.h"
#include "DiagnosticAutoMode.h"
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

            sel = 1 ;

            switch(sel) {
            case 0:
                mode = std::make_shared<AutoModeXero>(getRobot()) ;
                break ;

            case 1:
                mode = std::make_shared<DiagnosticAutoMode>(getRobot()) ;
                break ;
                
            case 8:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 1.0, 5.0) ;
                break ;
            case 9:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "Tune_Tune") ;
                break ;
            }
            setAction(mode) ;
        }
    }
}

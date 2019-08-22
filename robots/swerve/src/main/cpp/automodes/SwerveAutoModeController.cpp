#include "automodes/SwerveAutoModeController.h"
#include "automodes/TestSwerveDriveModuleAutomode.h"
#include "SwerveRobot.h"
#include <frc/DriverStation.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {
        SwerveAutoModeController::SwerveAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        SwerveAutoModeController::~SwerveAutoModeController() {
        }

        void SwerveAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                mode = std::make_shared<TestSwerveDriveModuleAutomode>(getRobot()) ;
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
                break ;

            case 7:
                break ;

            case 8:
                break ;

            case 9:
                break ;
            }
            setAction(mode) ;
        }
    }
}

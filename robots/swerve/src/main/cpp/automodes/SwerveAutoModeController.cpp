#include "automodes/SwerveAutoModeController.h"
#include "Swerve.h"
#include <tankdrive/modes/FollowPathAutomode.h>
#include <tankdrive/modes/ScrubCharMode.h>
#include <tankdrive/modes/StraightCharAutoMode.h>
#include <singlemotorsubsystem/SingleMotorPowerAutoMode.h>
#include <MessageLogger.h>
#include <frc/DriverStation.h>
#include <memory>

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
            auto &swerve = dynamic_cast<Swerve &>(getRobot()) ;
            auto robotsub = swerve.getSwerveSubsystem() ;
            auto motor1 = robotsub->getMotor1() ;
            auto motor2 = robotsub->getMotor2() ;
        
            sel = 1 ;

            switch(sel) {
            case 0:
                // mode = std::make_shared<SinglePowerAutoMode>(getRobot(), motor1, "mode1", "desc1", 0.25) ;
                break ;

            case 1:
                //mode = std::make_shared<SinglePowerAutoMode>(getRobot(), motor2, "mode1", "desc1", 0.25, 2.0) ;
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

#include "automodes/DroidAutoModeController.h"
#include "automodes/DroidAutoMode.h"
#include "automodes/DroidTestTuneMode.h"
#include "Droid.h"
#include <tankdrive/modes/FollowPathAutomode.h>
#include <MessageLogger.h>
#include <frc/DriverStation.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        DroidAutoModeController::DroidAutoModeController(Robot &robot) : AutoController(robot) {
            const char *key = "auto:testmode:active";

            if (robot.getSettingsParser().isDefined(key))
                testmode_ = robot.getSettingsParser().getBoolean(key);
        }
        
        DroidAutoModeController::~DroidAutoModeController() {
        }

        void DroidAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            auto &ds = frc::DriverStation::GetInstance();
            AutoModePtr mode = nullptr;

            if (!ds.IsFMSAttached() && testmode_)
            {
                //
                // If enabled in the params file and if we are not attached to the field
                //
                mode = std::make_shared<DroidTestTuneMode>(getRobot());
            }
            else
            {                
                switch(sel) 
                {
                case 0:
                    // Start side nearest port, ollect eight automode
                    break ;

                case 1:
                    // Start side fartherest from port, collect five automode
                    break ;

                case 2:
                    // Start center, score three in robot and move off line
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
            }

            setAction(mode) ;
        }
    }
}

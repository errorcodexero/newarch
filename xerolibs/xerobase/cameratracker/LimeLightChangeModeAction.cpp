#include "LimeLightChangeModeAction.h"
#include "CameraTracker.h"
#include "Robot.h"

namespace xero {
    namespace base {
            void LimeLightChangeModeAction::start() {
                limelight_.setMode(mode_) ;
                limelight_.setLED(led_) ;
                limelight_.setPipeline(pipeline_) ;
            }

            void LimeLightChangeModeAction::run() {            
            }

            bool LimeLightChangeModeAction::isDone() {
                return true ;
            }

            void LimeLightChangeModeAction::cancel() {
            }

            std::string LimeLightChangeModeAction::toString() {
                std::string result = "LimeLightChangeModeAction " ;
                result += LimeLightSubsystem::toString(mode_) ;
                result += " " ;
                result += LimeLightSubsystem::toString(led_) ;
                result += " " ;
                result += std::to_string(pipeline_) ;
                return result ;
            }
    }
}

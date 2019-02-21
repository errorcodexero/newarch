#include "CameraChangeAction.h"
#include "CameraTracker.h"
#include "Robot.h"

namespace xero {
    namespace base {
            void CameraChangeAction::start() {
                subsystem_.setCameraIndex(camera_) ;
                subsystem_.setCameraMode(mode_) ;
            }

            void CameraChangeAction::run() {            
            }

            bool CameraChangeAction::isDone() {
                return true ;
            }

            void CameraChangeAction::cancel() {
            }

            std::string CameraChangeAction::toString() {
                std::string result = "CameraChangeAction " ;
                result += std::to_string(camera_) ;
                result += " " ;
                result += CameraTracker::toString(mode_) ;
                return result ;
            }
    }
}

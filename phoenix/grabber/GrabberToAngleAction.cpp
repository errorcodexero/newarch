#include "GrabberToAngleAction.h"
#include "Grabber.h"
#include <Robot.h>
#include <cmath>

namespace xero {
    namespace phoenix {
        GrabberToAngleAction::GrabberToAngleAction(Grabber &g, double a) : GrabberAction(g) {            
            pid_ctrl_.initFromSettingsExtended(g.getRobot().getSettingsParser(), "grabber") ;
            threshold_ = g.getRobot().getSettingsParser().getDouble("grabber:threshold") ;
            target_ = a ;
        }

        GrabberToAngleAction::GrabberToAngleAction(Grabber &g, const std::string &name) : GrabberAction(g) {
            pid_ctrl_.initFromSettingsExtended(g.getRobot().getSettingsParser(), "grabber") ;           
            threshold_ = g.getRobot().getSettingsParser().getDouble("grabber:threshold") ;
            target_ = g.getRobot().getSettingsParser().getDouble(name) ;
        }        

        GrabberToAngleAction::~GrabberToAngleAction() {            
        }

        void GrabberToAngleAction::run() {
            double dt = getGrabber().getRobot().getDeltaTime() ;
            double current = getGrabber().getAngle() ;
            output_ = pid_ctrl_.getOutput(target_, current, 0, dt) ;

            getGrabber().motor_->Set(output_) ;
        }

        void GrabberToAngleAction::cancel() {
            target_ = getGrabber().getAngle() ;
        }

        bool GrabberToAngleAction::isAtTargetAngle() const {
            double diff = std::fabs(target_ - getGrabber().getAngle()) ;
            return diff < threshold_ ;
        }

        std::string GrabberToAngleAction::toString() {
            std::string result = "GrabberToAngle " + std::to_string(target_) ;
            return result ;
        }
    }
}
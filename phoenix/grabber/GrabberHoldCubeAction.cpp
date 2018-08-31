#include "GrabberHoldCubeAction.h"
#include "Grabber.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
            GrabberHoldCubeAction::GrabberHoldCubeAction(Grabber &grabber) : GrabberAction(grabber) {
                double hold_value_=getGrabber().getRobot().getSettingsParser().getDouble("grabber:hold");
            }
            GrabberHoldCubeAction::~GrabberHoldCubeAction(){

            }

            void GrabberHoldCubeAction::start(){
                getGrabber().motor_->Set(hold_value_);
            }
          
            void GrabberHoldCubeAction::run(){

            }

            bool GrabberHoldCubeAction::isDone(){
                return true;
            }

            void GrabberHoldCubeAction::cancel(){
                
            }

            std::string GrabberHoldCubeAction::toString(){

            }
    }
}
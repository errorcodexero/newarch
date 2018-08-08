#include "GrabberHoldCubeAction.h"
#include "Grabber.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
            GrabberHoldCubeAction(Grabber &grabber){
                double hold_value_=robot.getSettingsParser().getDouble("grabber:hold");
            }
            ~GrabberHoldCubeAction(){

            }

            void start(){
                getGrabber().motor_->Set(hold_value_);
            }
          
            void run(){

            }

            bool isDone(){
                return true;
            }

            void cancel(){
                
            }

            std::string toString(){

            }
    }
}
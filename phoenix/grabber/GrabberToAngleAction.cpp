#include "GrabberToAngleAction.h"
#include "Grabber.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {

            GrabberToAngleAction::GrabberToAngleAction(Grabber &grabber, double angle) : GrabberAction(grabber){
                angle_ = angle;
                angle_controller_.initFromSettingsExtended(getGrabber().getRobot().getSettingsParser(),"grabber");
                threshold_=getGrabber().getRobot().getSettingsParser().getDouble("grabber:threshold");
            }
            
            GrabberToAngleAction::~GrabberToAngleAction(){

            }

            void GrabberToAngleAction::start(){

            }

            void GrabberToAngleAction::run(){
                double dt = getGrabber().getRobot().getDeltaTime();
                double current = getGrabber().getAngle();
                double output = angle_controller_.getOutput(angle_, current, dt);
                getGrabber().motor_->Set(output);
            }

            bool GrabberToAngleAction::isDone(){

            }

            void GrabberToAngleAction::cancel(){

            }

           std::string GrabberToAngleAction::toString(){

           }
    }
}
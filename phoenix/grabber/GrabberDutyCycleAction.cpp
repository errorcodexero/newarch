#include "GrabberDutyCycleAction.h"
#include "Grabber.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
            GrabberDutyCycleAction::GrabberDutyCycleAction(Grabber &grabber, double value) : GrabberAction(grabber) {
                value_ = value;
            }

            GrabberDutyCycleAction::~GrabberDutyCycleAction(){

            }

            void GrabberDutyCycleAction::start() {
                getGrabber().motor_->Set(value_);
            }
          
            void GrabberDutyCycleAction::run() {

            }

            bool GrabberDutyCycleAction::isDone() {
                return true;
            }

            void GrabberDutyCycleAction::cancel() {

            }

            std::string GrabberDutyCycleAction::toString() {

            }
    }
}
#include "GrabberCalibrateAction.h"
#include "Grabber.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace phoenix {
            GrabberCalibrateAction::GrabberCalibrateAction(Grabber &grabber){
                calibratepower_ = grabber.getRobot().getSettingsParser().getDouble("grabber:calibrate:value");
                count_ = grabber.getRobot().getSettingsParser().getInteger("grabber:calibrate:count");
                diff_ = grabber.getRobot().getSettingsParser().getInteger("grabber:calibrate:diff");
            }
            GrabberCalibrateAction::~GrabberCalibrateActon(){

            }

            /// \brief Start the calibrate action.
            void GrabberCalibrateAction::start(){
                ticks_.clear();
            }

            /// \brief Run the duty cycle action.  This method does nothing.            
            void GrabberCalibrateAction::run(){
                ticks_.push_back(getGrabber().getEncoderTicks());
                if(ticks_.size()>count_){
                    ticks_.pop_front();
                }
            }

            /// \brief Signals if this action is done, always returns true    
            bool GrabberCalibrateAction::isDone(){

            }

            /// \brief Canel the current action, stops the motors and returns true
            void GrabberCalibrateAction::cancel(){

            }

            /// \brief Returns a human readable string for the action
            std::string GrabberCalibrateAction::toString(){

            }

    }
}
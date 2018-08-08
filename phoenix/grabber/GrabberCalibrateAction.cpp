#include "GrabberCalibrateAction.h"
#include "Grabber.h"
#include <Robot.h>
#include <numeric>
#include <algorithm>

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
                isdone_ = false;
            }

            /// \brief Run the duty cycle action.  This method does nothing.            
            void GrabberCalibrateAction::run(){
                ticks_.push_back(getGrabber().getEncoderTicks());
                if(ticks_.size()>count_){
                    ticks_.pop_front();
                }

                if(tick_.size()== count_){
                   auto miniter = std::min_element(ticks_.start(),ticks_.end());
                    auto maxiter = std::max_element(ticks_.start(),ticks_.end());
                    auto minval = *miniter;
                    auto maxval = *maxiter;
                    if(maxval-minval<diff_){
                        getGrabber().calibrate();
                        isdone_ = true;

                    }
                }
            }

            /// \brief Signals if this action is done, always returns true    
            bool GrabberCalibrateAction::isDone(){
                return isdone_;
            }

            /// \brief Canel the current action, stops the motors and returns true
            void GrabberCalibrateAction::cancel(){
                isdone_ = true;
            }

            /// \brief Returns a human readable string for the action
            std::string GrabberCalibrateAction::toString(){

            }

    }
}
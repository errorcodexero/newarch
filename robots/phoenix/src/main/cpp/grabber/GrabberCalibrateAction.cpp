#include "GrabberCalibrateAction.h"
#include "Grabber.h"
#include "phoenixgroups.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <algorithm>
#include <numeric>

using namespace xero::misc ;

namespace xero {
    namespace phoenix {

        std::string GrabberCalibrateAction::action_name("GrabberCalibration") ;

        GrabberCalibrateAction::GrabberCalibrateAction(Grabber &g) : GrabberAction(g) {
            calibrate_value_ = g.getRobot().getSettingsParser().getDouble("grabber:calibrate:value") ;
            count_ = static_cast<size_t>(g.getRobot().getSettingsParser().getInteger("grabber:calibrate:count")) ;
            calibrate_diff_ = static_cast<size_t>(g.getRobot().getSettingsParser().getInteger("grabber:calibrate:diff")) ;
        }

        GrabberCalibrateAction::~GrabberCalibrateAction() {            
        }

        void GrabberCalibrateAction::run() {
            if (!is_calibrated_) {
                getGrabber().motor_->Set(calibrate_value_) ;

                samples_.push_front(getGrabber().getEncoderValue()) ;
                while (samples_.size() > count_)
                    samples_.pop_back() ;

                if (samples_.size() == count_) {
                    int minval = *std::min_element(samples_.begin(), samples_.end()) ;
                    int maxval = *std::max_element(samples_.begin(), samples_.end()) ;

                    if (maxval - minval < static_cast<int>(calibrate_diff_)) {
                        //
                        // We are at our calibration location
                        //
                        double sum = std::accumulate(samples_.begin(), samples_.end(), 0) ;
                        calibration_location_ = static_cast<int>((sum + 0.5)/samples_.size()) ;
                        is_calibrated_ = true ;

                        //
                        // Push the calibration value to the grabber
                        //
                        MessageLogger &logger = getGrabber().getRobot().getMessageLogger() ;
                        logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_GRABBER) ;
                        logger << "Calibration complete, value is " << calibration_location_ ;
                        logger.endMessage() ;
                        
                        getGrabber().calibrate() ;
                    }
                }
            }
        }

        void GrabberCalibrateAction::cancel() {
            is_calibrated_ = true ;
        }
    }
}
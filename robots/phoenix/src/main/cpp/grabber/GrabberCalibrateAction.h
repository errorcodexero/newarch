#pragma once

#include "GrabberAction.h"
#include <list>

namespace xero {
    namespace phoenix {
        class GrabberCalibrateAction : public GrabberAction {
        public:
            GrabberCalibrateAction(Grabber &g) ;
            virtual ~GrabberCalibrateAction() ;

            int getCalibrationValue() const {
                return calibration_location_ ;
            }

            virtual void start();

            virtual void run() ;

            virtual void cancel() ;

            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;

        private:
            size_t count_ ;
            size_t calibrate_diff_ ;
            std::list<int> samples_ ;
            double calibrate_value_ ;
            bool is_calibrated_ ;
            int calibration_location_ ;
        } ;
    }
}
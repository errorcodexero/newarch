#include "TurntableCalibrateAction.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cstdlib>

using namespace xero::misc ;

namespace xero {
    namespace phaser {

        std::string TurntableCalibrateAction::action_name("TurntableCalibrate") ;

        TurntableCalibrateAction::TurntableCalibrateAction(Turntable &turntable) : TurntableAction(turntable) {            
            power_ = turntable.getRobot().getSettingsParser().getDouble("turntable:calibrate:power") ;
            encbase_ = turntable.getRobot().getSettingsParser().getInteger("turntable:calibrate:encbase") ;
            samples_ = turntable.getRobot().getSettingsParser().getInteger("turntable:calibrate:samples") ;
            threshold_ = turntable.getRobot().getSettingsParser().getInteger("turntable:calibrate:threshold") ;            
        }

        TurntableCalibrateAction::~TurntableCalibrateAction() {            
        }

        void TurntableCalibrateAction::start() {
            getTurntable().setMotorPower(-0.1) ;
            is_done_ = false ;
        }


        void TurntableCalibrateAction::getMinMax(int &minval, int &maxval) {
            minval = std::numeric_limits<int>::max() ;
            maxval = std::numeric_limits<int>::min() ;

            for(const int &d : counts_) {
                if (d < minval)
                    minval = d ;

                if (d > maxval)
                    maxval = d ;
            }
        }        

        void TurntableCalibrateAction::run() {
            int minval, maxval ;            
            counts_.push_back(getTurntable().getEncoderValue()) ;
            if (counts_.size() > static_cast<size_t>(samples_))
                counts_.pop_front() ;

            getMinMax(minval, maxval) ;
            if (counts_.size() == static_cast<size_t>(samples_) && maxval - minval <= threshold_) {
                getTurntable().calibrate(encbase_) ;
                is_done_ = true ;
            }                
        }

        bool TurntableCalibrateAction::isDone() {
            return is_done_ ;
        }
    }
}
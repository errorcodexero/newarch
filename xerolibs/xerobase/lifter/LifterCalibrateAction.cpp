#include "LifterCalibrateAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {
        LifterCalibrateAction::LifterCalibrateAction(Lifter &lifter) : LifterAction(lifter) {
            power_ = lifter.getRobot().getSettingsParser().getDouble("lifter:calibrate:power") ;
            encbase_ = lifter.getRobot().getSettingsParser().getInteger("lifter:calibrate:encbase") ;
            samples_ = lifter.getRobot().getSettingsParser().getInteger("lifter:calibrate:samples") ;
            threshold_ = lifter.getRobot().getSettingsParser().getInteger("lifter:calibrate:threshold") ;
        }

        LifterCalibrateAction::~LifterCalibrateAction() {            
        }

        std::string LifterCalibrateAction::toString() {
            std::string result = "LifteRToBottom " + std::to_string(power_) ;
            return result ;
        }

        void LifterCalibrateAction::start() {
            is_done_ = false ;
            getLifter().setMotorPower(power_) ;
        }

        void LifterCalibrateAction::getMinMax(int &minval, int &maxval) {
            minval = std::numeric_limits<int>::max() ;
            maxval = std::numeric_limits<int>::min() ;

            for(const int &d : counts_) {
                if (d < minval)
                    minval = d ;

                if (d > maxval)
                    maxval = d ;
            }
        }

        void LifterCalibrateAction::run() {
            if (!is_done_) {
                int minval, maxval ;
                counts_.push_back(getLifter().getEncoderValue()) ;
                if (counts_.size() > static_cast<size_t>(samples_))
                    counts_.pop_front() ;

                MessageLogger &logger = getLifter().getRobot().getMessageLogger() ;
                logger << "lifter samples:" ;
                for(int d: counts_)
                    logger << " " << d ;
                logger.endMessage() ;

                getMinMax(minval, maxval) ;
                if (counts_.size() == static_cast<size_t>(samples_) && maxval - minval <= threshold_) {
                    getLifter().calibrate(encbase_) ;
                    is_done_ = true ;
                }
            }
        }
    }
}

#pragma once

#include <Robot.h>

#include "TubToucherAction.h"
#include "TubToucher.h"

namespace xero {
    namespace ranseur {
        class TubToucherDeployAction : public TubToucherAction {
        public:
            /// Creates a TubToucherDeployAction.
            /// @param tubtoucher The bunny arm subsystem.
            /// @param raise True if the arm should be raised, false if it should be lowered.
            TubToucherDeployAction(TubToucher &tubtoucher, bool raise) : TubToucherAction(tubtoucher), raise_(raise) { 
                isDone_ = false;      
                duration_ = tubtoucher.getRobot().getSettingsParser().getDouble("tubtoucher:deploy:time");         
            }
            
            virtual ~TubToucherDeployAction() {                
            }

            virtual void start() { 
                isDone_ = false ;
                TubToucher &tubtoucher = getTubToucher() ;
                tubtoucher.solenoid_->Set(raise_);

                // Determine the end time.
                endTime_ = tubtoucher.getRobot().getTime() + duration_;
            }

            virtual void run() {
                // we're done once we pass the end time
                isDone_ = (getTubToucher().getRobot().getTime() > endTime_);
            }

            virtual bool isDone() {
                return isDone_ ;
            }

            virtual void cancel() {
                isDone_ = true ;
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            bool isDone_;
            double endTime_;
            double duration_;
            bool raise_;
            static const std::string action_name ;
        } ;
    }
}

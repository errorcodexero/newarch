#pragma once

#include <Robot.h>

#include "TubToucherAction.h"
#include "TubToucher.h"
#include <MessageLogger.h>

namespace xero {
    namespace ranseur {
        class TubToucherDeployAction : public TubToucherAction {
        public:
            /// Creates a TubToucherDeployAction.
            /// @param tubtoucher The bunny arm subsystem.
            /// @param raise True if the arm should be raised, false if it should be lowered.
            TubToucherDeployAction(TubToucher &tubtoucher, bool raise) : TubToucherAction(tubtoucher), raise_(raise) { 
                duration_ = tubtoucher.getRobot().getSettingsParser().getDouble("tubtoucher:deploy:time");         
            }
            
            virtual ~TubToucherDeployAction() {                
            }

            virtual void start() {
                TubToucherAction::start();
                TubToucher &tubtoucher = getTubToucher() ;

                if (!raise_)
                {
                    tubtoucher.solenoid1_1_->Set(false);
                    tubtoucher.solenoid1_2_->Set(true);
                    tubtoucher.solenoid2_1_->Set(false);
                    tubtoucher.solenoid2_2_->Set(true);
                    setDone();
                }
                else
                {
                    tubtoucher.solenoid1_1_->Set(true);
                    tubtoucher.solenoid1_2_->Set(false);
                    tubtoucher.solenoid2_1_->Set(true);
                    tubtoucher.solenoid2_2_->Set(false);   
                    endTime_ = tubtoucher.getRobot().getTime() + duration_;           
                }
                tubtoucher.deployed_ = raise_ ;
            }

            virtual void run() {
                TubToucherAction::run() ;

                // we're done once we pass the end time
                if (getTubToucher().getRobot().getTime() > endTime_)
                {
                    auto &logger = getTubToucher().getRobot().getMessageLogger() ;
                    logger.startMessage(xero::misc::MessageLogger::MessageType::debug) ;
                    logger << "Disengaged tub touchers" ;
                    logger.endMessage() ;

                    TubToucher &tubtoucher = getTubToucher() ;                    
                    tubtoucher.solenoid1_1_->Set(false);
                    tubtoucher.solenoid1_2_->Set(false);
                    tubtoucher.solenoid2_1_->Set(false);
                    tubtoucher.solenoid2_2_->Set(false);

                    setDone();
                }
            }


            virtual void cancel() {
                TubToucherAction::cancel();
                setDone();
            }

            virtual std::string toString() {
                std::string ret ;

                if (raise_)
                    ret = "TubToucherDeployAction - extend" ;
                else
                    ret = "TubToucherDeployAction - retract" ;

                return ret ;
            }

        private:
            double endTime_;
            double duration_;
            bool raise_;
        } ;
    }
}

#pragma once

#include <Robot.h>

#include "BunnyArmAction.h"
#include "BunnyArm.h"

namespace xero {
    namespace ranseur {
        class BunnyArmDeployAction : public BunnyArmAction {
        public:
            /// Creates a BunnyArmDeployAction.
            /// @param bunnyArm The bunny arm subsystem.
            /// @param raise True if the arm should be raised, false if it should be lowered.
            BunnyArmDeployAction(BunnyArm &bunnyArm, bool raise) : BunnyArmAction(bunnyArm), raise_(raise) { 
                duration_ = bunnyArm.getRobot().getSettingsParser().getDouble("bunnyarm:deploy:time");         
            }
            
            virtual ~BunnyArmDeployAction() {                
            }

            virtual void start() {
                GenericAction::start() ;

                BunnyArm &bunnyArm = getBunnyArm() ;
                if (raise_)
                    bunnyArm.raise() ;
                else
                    bunnyArm.lower() ;

                // Determine the end time.
                endTime_ = bunnyArm.getRobot().getTime() + duration_;
            }

            virtual void run() {
                // we're done once we pass the end time
                if (getBunnyArm().getRobot().getTime() > endTime_)
                    setDone();
            }

            virtual void cancel() {
                setDone();
            }

            virtual std::string toString() {
                std::string ret ;

                if (raise_)
                    ret = "BunnyArmDeployAction - raise arm" ;
                else
                    ret = "BunnyArmDeployAction - lower arm" ;

                return ret ;
            }

        private:
            double endTime_;
            double duration_;
            bool raise_;
        } ;
    }
}

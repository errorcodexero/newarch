#pragma once

#include "PhoenixSubsystemAction.h"

namespace xero {
    namespace phoenix {
        class DriveAndCollectAction : public PhoenixSubsystemAction {
        public:
            DriveAndCollectAction(PhoenixRobotSubsystem &sub, double distance) ;
            DriveAndCollectAction(PhoenixRobotSubsystem &sub, const std::string &distance) ;            
            virtual ~DriveAndCollectAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone()  ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                Driving,
                Stopping,
                Collected,
                DistanceReached,
                Cancel
            } ;

        private:
            State state_ ;
            double distance_ ;
            double start_ ;
            xero::base::ActionPtr stop_drivebase_ ;
        } ;
    }
}

#pragma once

#include "TankDriveAction.h"

namespace xero {
    namespace base {
        class TankDriveTimedPowerAction : public TankDriveAction {
        public:
            TankDriveTimedPowerAction(TankDrive &db, double left, double right, double time) ;
            TankDriveTimedPowerAction(TankDrive &db, const std::string &left, const std::string &right, const std::string &time) ;
            virtual ~TankDriveTimedPowerAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;   
                        
        private:
            double left_ ;
            double right_ ;
            double duration_ ;
            double start_ ;
            bool isdone_ ;
        } ;
    }
}

#pragma once

#include "TankDriveAction.h"

namespace xero {
    namespace base {
        class TankDrivePowerAction : public TankDriveAction {
        public:
            TankDrivePowerAction(TankDrive &db, double left, double right) ;
            TankDrivePowerAction(TankDrive &db, const std::string &left, const std::string &right) ;
            virtual ~TankDrivePowerAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;   
                        
        private:
            double left_ ;
            double right_ ;
        } ;
    }
}
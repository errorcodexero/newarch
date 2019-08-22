#pragma once

#include <rev/CANSparkMax.h>
#include <memory>

namespace xero
{
    namespace base
    {
        class SwerveModule
        {
        public:
            SwerveModule(int rotate, int drive) ;
            virtual ~SwerveModule() ;

            void setRotatePower(double p) {
                rotate_->Set(p) ;
            }

            void setDrivePower(double p) {
                drive_->Set(p) ;
            }
            
        private:
            std::shared_ptr<rev::CANSparkMax> rotate_ ;
            std::shared_ptr<rev::CANSparkMax> drive_ ;
        } ;
    }
}
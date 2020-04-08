#pragma once

#include <Subsystem.h>
#include <memory>
#include <frc/Solenoid.h>

namespace xero {
    namespace ranseur {

        class TubToucher : public xero::base::Subsystem {
            friend class TubToucherDeployAction ;

        public:
            TubToucher(xero::base::Subsystem *parent) ;

            virtual ~TubToucher() ;

            virtual void computeMyState() ;

            bool isDeployed() {
                return deployed_ ;
            }

            virtual void init(xero::base::LoopType lt) ;

        protected:
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool deployed_ ;

            std::shared_ptr<frc::Solenoid> solenoid1_1_ ;
            std::shared_ptr<frc::Solenoid> solenoid1_2_ ;            
            std::shared_ptr<frc::Solenoid> solenoid2_1_; 
            std::shared_ptr<frc::Solenoid> solenoid2_2_;                       
        } ;
    }
}

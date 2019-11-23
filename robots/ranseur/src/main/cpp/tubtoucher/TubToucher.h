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

            virtual void computeState() ;

        protected:
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool deployed_ ;

            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}

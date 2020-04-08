#pragma once

#include <Subsystem.h>
#include <frc/Relay.h>
#include <memory>

namespace xero
{
    namespace droid
    {
        class BlinkySubsystem: public xero::base::Subsystem
        {
        public:
            BlinkySubsystem(xero::base::Subsystem *parent) ;
            virtual ~BlinkySubsystem() ;

            void init(xero::base::LoopType ltype) override ;            

            void computeMyState() override ;
            void run() override ;

            bool canAcceptAction(xero::base::ActionPtr action) override ;

            void setPattern(uint8_t pattern) ;

        private:
            std::shared_ptr<frc::Relay> r0_ ;
            std::shared_ptr<frc::Relay> r1_ ;
        } ;
    }
}
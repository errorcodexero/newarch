#pragma once

#include <actions/Action.h>

namespace xero
{
    namespace droid
    {
        class BlinkySubsystem ;

        class BlinkyAction : public xero::base::Action        
        {
        public:
            enum class Pattern : uint8_t
            {
                Off = 0,
                FiveBalls = 1,
                ThirtySeconds = 2,
                TraverseLeft = 3,
                TraverseRight = 4,
                FunPatternOne = 5,
                FunPatternTwo = 6,
                FunPatternThree = 7,
                FunPatternFour = 8,
                RandomPattern=255
            } ;

        public:
            BlinkyAction(BlinkySubsystem &sub, Pattern pattern) ;
            virtual ~BlinkyAction() ;

            void start() override ;

        private:
            Pattern pattern_ ;
            BlinkySubsystem &subsystem_ ;
        } ;
    }
}
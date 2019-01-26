#pragma once

#include "ActionSequence.h"

namespace xero 
{
    namespace base 
    {
        class DetectAutoSequence
        {
        public:
            DetectAutoSequence(ActionSequencePtr seq) {
                sequence_ = seq ;
            }

            ~DetectAutoSequence() {                
            }
            
            virtual bool isTakeoverValid() = 0 ;

            ActionSequencePtr getSequence() {
                return sequence_ ;
            }

        private:
            ActionSequencePtr sequence_ ;
        };
    }
}
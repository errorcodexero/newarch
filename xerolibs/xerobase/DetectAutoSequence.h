#pragma once

#include "Action.h"

namespace xero 
{
    namespace base 
    {
        class DetectAutoSequence
        {
        public:
            DetectAutoSequence(ActionPtr seq) {
                sequence_ = seq ;
            }

            ~DetectAutoSequence() {                
            }
            
            virtual bool isTakeoverValid() = 0 ;

            ActionPtr getSequence() {
                return sequence_ ;
            }

        private:
            ActionPtr sequence_ ;
        };
    }
}
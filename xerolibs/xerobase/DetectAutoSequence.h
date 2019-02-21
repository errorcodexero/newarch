#pragma once

#include "Action.h"
#include <memory>

namespace xero 
{
    namespace base 
    {
        class TeleopController ;

        class DetectAutoSequence
        {
        public:
            DetectAutoSequence(std::shared_ptr<TeleopController> teleop, ActionPtr seq) {
                sequence_ = seq ;
                teleop_ = teleop ;
            }

            ~DetectAutoSequence() {                
            }
            
            virtual bool isTakeoverValid() = 0 ;

            ActionPtr getSequence() {
                return sequence_ ;
            }

            void removeMe() ;

        private:
            ActionPtr sequence_ ;
            std::shared_ptr<TeleopController> teleop_ ;
        };
    }
}
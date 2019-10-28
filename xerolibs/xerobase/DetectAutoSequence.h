#pragma once

#include "actions/Action.h"
#include <memory>

namespace xero 
{
    namespace base 
    {
        class TeleopController ;

        class DetectAutoSequence
        {
        public:
            DetectAutoSequence(const std::string &name, std::shared_ptr<TeleopController> teleop, ActionPtr seq) ;

            virtual ~DetectAutoSequence() ;
            
            virtual bool isTakeoverValid() = 0 ;

            ActionPtr getSequence() {
                return sequence_ ;
            }

            const std::string &getName() {
                return name_ ;
            }

        private:
            std::string name_ ;
            ActionPtr sequence_ ;
            std::shared_ptr<TeleopController> teleop_ ;
        };
    }
}
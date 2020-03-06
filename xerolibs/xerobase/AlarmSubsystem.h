#pragma once

#include "Subsystem.h" 
#include "LoopType.h"
#include <iostream>
#include <vector>

/// \file

namespace xero {
    namespace base {

        class AlarmSubsystem : public Subsystem {\
        public:
            /// \brief Create a new subsystem object
            AlarmSubsystem(Subsystem *parent) ;

            /// \brief destroy the subsystem
            virtual ~AlarmSubsystem();

            void setAlarm(LoopType lt, double remaining) ;

            virtual void run() ;

        private:
            std::vector<std::pair<LoopType, double>> alarms_ ;
        }  ;
    }
}

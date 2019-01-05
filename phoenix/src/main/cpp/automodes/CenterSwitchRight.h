#pragma once

#include "PhoenixAutoMode.h"

namespace xero {
    namespace phoenix {
        class CenterSwitchRight : public PhoenixAutoMode
        {
        public:
            CenterSwitchRight(Phoenix &robot) ;
            virtual ~CenterSwitchRight() ;

        private:
            void create() ;
        } ;
    }
}
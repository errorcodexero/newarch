#pragma once

#include "PhoenixAutoMode.h"

namespace xero {
    namespace phoenix {
        class FollowPathAutoMode : public PhoenixAutoMode
        {
        public:
            FollowPathAutoMode(Phoenix &robot) ;
            virtual ~FollowPathAutoMode() ;

        private:
            void create() ;
        } ;
    }
}
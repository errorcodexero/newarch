#pragma once

#include "XeroPathManager.h"
#include <networktables/NetworkTableInstance.h>

namespace xero
{
    namespace base
    {
        class NTPathDataWatcher
        {
        public:
            NTPathDataWatcher(xero::misc::XeroPathManager &mgr) ;
            virtual ~NTPathDataWatcher() ;

            void update() ;

        private:
            xero::misc::XeroPathManager &mgr_ ;
        } ;
    }
}
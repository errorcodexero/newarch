#pragma once
#include "MessageLogger.h"
#include "XeroPathManager.h"
#include <networktables/NetworkTableInstance.h>

namespace xero
{
    namespace base
    {
        class NTPathDataWatcher
        {
        public:
            NTPathDataWatcher(xero::misc::MessageLogger &logger, xero::misc::XeroPathManager &mgr) ;
            virtual ~NTPathDataWatcher() ;

            void update() ;

        private:
            xero::misc::XeroPathManager &mgr_ ;
            xero::misc::MessageLogger &logger_ ;
        } ;
    }
}
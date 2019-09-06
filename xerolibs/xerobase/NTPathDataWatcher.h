#pragma once
#include "MessageLogger.h"
#include "XeroPathManager.h"
#include <networktables/NetworkTableInstance.h>
#include <mutex>

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

            uint32_t calcHash(const char *str, uint32_t orig) ;

        private:
            void keyUpdate(const wpi::StringRef &key) ;
            static void listener(nt::NetworkTable *table, wpi::StringRef name, nt::NetworkTableEntry entry, std::shared_ptr<nt::Value> value, int flags) ;


        private:
            xero::misc::XeroPathManager &mgr_ ;
            xero::misc::MessageLogger &logger_ ;
            std::mutex keylock_ ;
            std::list<std::string> keys_ ;
            std::map<std::string, uint32_t> hashes_ ;
        } ;
    }
}
#include "NTPathDataWatcher.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <cassert>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        NTPathDataWatcher::NTPathDataWatcher(MessageLogger &logger, XeroPathManager &mgr) : mgr_(mgr), logger_(logger)
        {
        }

        NTPathDataWatcher::~NTPathDataWatcher()
        {
        }

        void NTPathDataWatcher::keyUpdate(const wpi::StringRef &key)
        {
        }

        uint32_t NTPathDataWatcher::calcHash(const char *str, uint32_t orig)
        {
            if (orig == 0)
                orig = 5381 ;

            while (char c = *str++)
                orig = ((orig << 5) + orig) + c; /* hash * 33 + c */

            return orig ;
        }

        void NTPathDataWatcher::update()
        {
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto table = inst.GetTable("/XeroPaths") ;

            std::list<std::string> names = mgr_.getNames() ;
            for(auto name : names)
            {
                std::string left = table->GetString(name + "_left", "") ;
                std::string right = table->GetString(name + "_right", "") ;

                if (left.length() > 0 && right.length() > 0)
                {
                    uint32_t hash = calcHash(left.c_str(), 0) ;
                    hash = calcHash(right.c_str(), hash) ;

                    auto it = hashes_.find(name) ;
                    if (it == hashes_.end() || it->second != hash)
                    {
                        mgr_.replaceData(name, left, right) ;
                        logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PATHWATCHER) ;
                        logger_ << "updated path '" << name ;
                        logger_.endMessage() ;
                        hashes_[name] = hash ;
                    }
                }
            }
        }
    }
}

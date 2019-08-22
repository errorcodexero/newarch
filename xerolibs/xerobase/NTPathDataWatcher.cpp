#include "NTPathDataWatcher.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <cassert>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        NTPathDataWatcher *NTPathDataWatcher::theOne = nullptr ;

        void NTPathDataWatcher::listener(nt::NetworkTable *table, wpi::StringRef name, nt::NetworkTableEntry entry, std::shared_ptr<nt::Value> value, int flags)
        {
            theOne->keyUpdate(name) ;
        }

        NTPathDataWatcher::NTPathDataWatcher(MessageLogger &logger, XeroPathManager &mgr) : mgr_(mgr), logger_(logger)
        {
            assert(theOne == nullptr) ;

            theOne = this ;
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto table = inst.GetTable("XeroPaths") ;

            table->AddEntryListener(listener, NT_NOTIFY_IMMEDIATE | NT_NOTIFY_NEW | NT_NOTIFY_UPDATE) ;
        }

        NTPathDataWatcher::~NTPathDataWatcher()
        {
            assert(theOne == this) ;
            theOne = nullptr ;
        }

        void NTPathDataWatcher::keyUpdate(const wpi::StringRef &key)
        {
            std::string name = key ;
            if (name.length() > 5 && name.substr(name.length() - 5, 5) == "_main")
            {
                name = name.substr(0, name.length() - 5) ;
                keylock_.lock() ;
                keys_.push_back(name) ;
                keylock_.unlock() ;
            }
        }

        void NTPathDataWatcher::update()
        {
            keylock_.lock() ;
            if (keys_.size() > 0)
            {
                nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
                auto table = inst.GetTable("XeroPaths") ;

                while (keys_.size() > 0)
                {
                    std::string name = keys_.front() ;
                    keys_.pop_front() ;

                    std::string left = table->GetString(name + "_left", "") ;
                    std::string right = table->GetString(name + "_right", "") ;

                    if (left.length() > 0 && right.length() > 0)
                    {
                        mgr_.replaceData(name, left, right) ;
                        logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PATHWATCHER) ;
                        logger_ << "updated path '" << name ;
                        logger_.endMessage() ;
                    }
                }
            }
            keylock_.unlock() ;
        }
    }
}

#include "NTPathDataWatcher.h"
#include <networktables/NetworkTableInstance.h>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        NTPathDataWatcher::NTPathDataWatcher(XeroPathManager &mgr) : mgr_(mgr)
        {
        }

        NTPathDataWatcher::~NTPathDataWatcher()
        {
        }

        void NTPathDataWatcher::update()
        {
            nt::NetworkTableInstance inst = nt::NetworkTableInstance::GetDefault() ;
            auto table = inst.GetTable("XeroPaths") ;

            std::list<std::string> names = mgr_.getNames() ;
            for(const std::string &name : names)
            {
                std::string left = table->GetString(name + "_left", "") ;
                std::string right = table->GetString(name + "_right", "") ;
                if (left.length() > 0 && right.length() > 0)
                {
                    mgr_.replaceData(name, left, right) ;
                }
            }
        }
    }
}
#pragma once

#include <AutoMode.h>


namespace xero
{
    namespace droid
    {
        class DroidSubsystem;
        class DroidAutoMode : public xero::base::AutoMode
        {
        public:
            DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc);
            virtual ~DroidAutoMode();

            std::shared_ptr<DroidSubsystem> getDroidSubsystem();
        };
    }
}
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
            DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc,
                          const std::string &configFileName);
            virtual ~DroidAutoMode();

            void start() override;

            std::shared_ptr<DroidSubsystem> getDroidSubsystem();

        private:
            double startX_;
            double startY_;
            double startAngle_;
        };
    }
}
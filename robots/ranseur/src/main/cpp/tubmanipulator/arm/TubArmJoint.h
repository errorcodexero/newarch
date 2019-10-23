#pragma once

#include <Subsystem.h>
#include <PIDCtrl.h>

#include <frc/Encoder.h>

#include <map>

namespace xero {
    namespace ranseur {
        class TubArmJoint : public xero::base::SingleMotorSubsystem {
        public:
            TubArmJoint(Robot &robot, 
                        const std::string &name, 
                        const std::string &configName,
                        const std::vector<std::string> positions,
                        uint64_t id);

        private:
            std::map<std::string, int> positions_;
        };
    }
}
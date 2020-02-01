#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include "Turret.h"

namespace xero {
    namespace droid {
        class TurretStopAction : public xero::base::SingleMotorPowerAction {
        public:
            TurretStopAction(Turret &turret): SingleMotorPowerAction(turret, 0.0), turret_(turret) {}

            void start() override {
                turret_.readyToFire_ = false;
                SingleMotorPowerAction::start();
            }
        private:
            Turret &turret_;
        };
    }
}

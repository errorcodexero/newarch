#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include "limelight/DroidLimeLight.h"
#include "Turret.h"
#include "Droid.h"

namespace xero {
    namespace droid {
        class TurretStopAction : public xero::base::SingleMotorPowerAction {
        public:
            TurretStopAction(Turret &turret): SingleMotorPowerAction(turret, 0.0), turret_(turret) {}

            void start() override {
                SingleMotorPowerAction::start();
                turret_.readyToFire_ = false;                
            }
        private:
            Turret &turret_;
        };
    }
}

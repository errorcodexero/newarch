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
                turret_.readyToFire_ = false;
                
                auto ll = static_cast<Droid&>(turret_.getRobot()).getDroidSubsystem();
                ll->getLimeLight()->setLedMode(xero::base::LimeLight::ledMode::ForceOn) ;
                SingleMotorPowerAction::start();
            }
        private:
            Turret &turret_;
        };
    }
}

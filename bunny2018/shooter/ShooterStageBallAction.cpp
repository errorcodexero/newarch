#include <Robot.h>
#include "ShooterStageBallAction.h"

namespace xero {
    namespace bunny2018 {
        
            ShooterStageBallAction::ShooterStageBallAction(Shooter& shooter) : ShooterAction(shooter) {
            }

            ShooterStageBallAction::~ShooterStageBallAction() {
            }

            void ShooterStageBallAction::start() {
            }

            void ShooterStageBallAction::run() {
            }

            bool ShooterStageBallAction::isDone() {
                return getSubsystem().getBallIsStaged();
            }

            void ShooterStageBallAction::cancel() {
            }

            std::string ShooterStageBallAction::toString() {
                std::string result = "ShooterStageBallAction staged=" ;
				if (getSubsystem().getBallIsStaged())
					result += "true" ;
				else
					result += "false" ;

                return result ;
            }
    }
}

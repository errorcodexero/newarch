#include <Robot.h>
#include "ShooterEjectOneBallAction.h"

namespace xero {
    namespace bunny2018 {
        
            ShooterEjectOneBallAction::ShooterEjectOneBallAction(Shooter& shooter) : ShooterAction(shooter) {
            }

            ShooterEjectOneBallAction::~ShooterEjectOneBallAction() {
            }

            void ShooterEjectOneBallAction::start() {
            }

            void ShooterEjectOneBallAction::run() {
            }

            bool ShooterEjectOneBallAction::isDone() {
                
            }

            void ShooterEjectOneBallAction::cancel() {
            }

            std::string ShooterEjectOneBallAction::toString() {
                std::string result = "ShooterEjectOneBallAction";
                
                return result ;
            }
    }
}

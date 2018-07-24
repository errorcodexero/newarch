#include "WingsModel.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            WingsModel::WingsModel() {
                wings_state_ = false ;
            }

            WingsModel::~WingsModel() {                
            }

            std::string WingsModel::toString() {
                std::string result("wings: ") ;
                if (wings_state_)
                    result += "deployed" ;
                else
                    result += "undeployed" ;

                return result ;
            }

            void WingsModel::run(double dt) {
                if (wings_solenoid_state)
                    wings_state_ = true ;
            }

            void WingsModel::inputChanged(SimulatedObject *obj) {
                Solenoid *sol = dynamic_cast<Solenoid *>(obj) ;
                if (sol != nullptr)
                    wings_solenoid_state = sol->Get() ;
            }

            void WingsModel::addSolenoid(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == 1) {
                    wings_ = sol ;
                    wings_->addModel(this) ;
                }
            }
        }
    }
}

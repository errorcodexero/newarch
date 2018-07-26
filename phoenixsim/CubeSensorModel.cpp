#include "CubeSensorModel.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            CubeSensorModel::CubeSensorModel() {
                cube_sensed_ = false ;
            }

            CubeSensorModel::~CubeSensorModel() {                
            }

            std::string CubeSensorModel::toString() {
                std::string result("cube sensor: ") ;

                result += "cube " ;
                result += (cube_sensed_ ? "present" : "notpresent") ;

                return result ;
            }

            void CubeSensorModel::run(double dt) {
            }

	        void CubeSensorModel::inputChanged(SimulatedObject *obj) {              
            }

            void CubeSensorModel::addDigitalInput(frc::DigitalInput *input) {
                if (input->GetChannel() == 12) {
                    input_ = input ;
                    input_->SimulatorSetValue(false) ;
                    input_->addModel(this) ;
                }
            }
        }
    }
}

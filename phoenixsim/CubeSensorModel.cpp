#include "CubeSensorModel.h"
#include <RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            CubeSensorModel::CubeSensorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "cubesensor") {
                cube_sensed_ = false ;
                cube_sensor_input_ = simbase.getSettingsParser().getInteger("hw:collector:cubesensor") ;
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
                if (input->GetChannel() == cube_sensor_input_) {
                    input_ = input ;
                    input_->SimulatorSetValue(false) ;
                    input_->addModel(this) ;
                }
            }
        }
    }
}

#include "CargoHolderModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace phaser {
            CargoHolderModel::CargoHolderModel(RobotSimBase &simbase) : SubsystemModel(simbase, "cargoholder") {

                motor_channel_ = simbase.getSettingsParser().getInteger("hw:cargoholder:motor") ;
                cargo_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:cargoholder:sensor") ;

                has_cargo_ = false ;
                power_ = 0.0 ;
            }

            CargoHolderModel::~CargoHolderModel() {
            }

            void CargoHolderModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                lines.push_back("  Motor: " + std::to_string(getPower())) ;
                line = "  Cargo: " ;
                line += (hasCargo() ? "YES" : "NO") ;
                lines.push_back(line) ;            
            }

            std::string CargoHolderModel::toString() {
                std::string result("cargoholder: ") ;
                return result ;
            }

            void CargoHolderModel::run(double dt) {
            }

            void CargoHolderModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr)
                    power_ = talon->Get() ;
            }

            void CargoHolderModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor->addModel(this) ;
                }            
            }
            
            void CargoHolderModel::addDigitalInput(frc::DigitalInput *input)  {
                if (input->GetChannel() == cargo_sensor_channel_) {
                    cargo_sensor_ = input ;
                    cargo_sensor_->addModel(this) ;
                    cargo_sensor_->SimulatorSetValue(true) ;
                }
            }
        }
    }
}

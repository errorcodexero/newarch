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

                cargo_sensor_ = nullptr ;
            }

            CargoHolderModel::~CargoHolderModel() {
            }

            bool CargoHolderModel::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "cargo") {
                    ret = true ;
                    has_cargo_ = (value ? true : false) ;
                    if (cargo_sensor_ != nullptr)
                        cargo_sensor_->SimulatorSetValue(has_cargo_) ;
                }

                return ret ;
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
                VictorSPX *victor = dynamic_cast<VictorSPX *>(obj) ;
                if (victor != nullptr)
                    power_ = victor->Get() ;
            }

            void CargoHolderModel::addDevice(VictorSPX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor->addModel(this) ;
                }            
            }
            
            void CargoHolderModel::addDevice(frc::DigitalInput *input)  {
                if (input->GetChannel() == cargo_sensor_channel_) {
                    cargo_sensor_ = input ;
                    cargo_sensor_->addModel(this) ;
                    cargo_sensor_->SimulatorSetValue(has_cargo_) ;
                }
            }
        }
    }
}

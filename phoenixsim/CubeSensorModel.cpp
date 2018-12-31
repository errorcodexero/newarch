#include "CubeSensorModel.h"
#include "PhoenixSimulator.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            CubeSensorModel::CubeSensorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "cubesensor") {
                cube_sensed_ = false ;
                cube_sensor_input_ = simbase.getSettingsParser().getInteger("hw:collector:cubesensor") ;
                motor_channel_1_ = simbase.getSettingsParser().getInteger("hw:intake:leftmotor") ;
                motor_channel_2_ = simbase.getSettingsParser().getInteger("hw:intake:rightmotor") ;         
                eject_duration_ = simbase.getSettingsParser().getDouble("collector:eject_duration") ;       
                inited_ = false ;
                last_time_ = 0.0 ;
                input_ = nullptr ;
                running_ = false ;
            }

            CubeSensorModel::~CubeSensorModel() {                
            }

            std::string CubeSensorModel::toString() {
                std::string result("cube sensor: ") ;

                result += "cube " ;
                result += (cube_sensed_ ? "present" : "notpresent") ;

                return result ;
            }


            void CubeSensorModel::init() {
                if (getSimulator().hasProperty("cube")) {
                    std::vector<double> values ;
                    const std::string &prop = getSimulator().getProperty("cube") ;
                    if (parseDoubleList(prop, values)) {
                        for(double t : values) {
                            OnTime tm(t) ;
                            ontimes_.push_back(tm) ;
                        }
                    }
                }
            }

            void CubeSensorModel::evalSensor() {
                if (!running_) {
                    if (duty_ < -0.1) {
                        running_ = true ;
                        start_ = getSimulator().getTime() ;
                    }
                }
                else {
                    if (duty_ >= -0.1)
                        running_ = false  ;
                    else if (getSimulator().getTime() > start_ + eject_duration_) {
                        //
                        // The cube is ejected
                        //
                        running_ = false ;
                        cube_sensed_ = false ;
                    }
                }
            }

            void CubeSensorModel::run(double dt) {
                auto &phoenix = dynamic_cast<PhoenixSimulator &>(getSimulator()) ;

                evalSensor() ;

                double now = phoenix.getTime() ;

                if (cube_sensed_ == false) {
                    //
                    // Cubes based on time
                    //
                    for(const auto &entry: ontimes_) {
                        if (entry.start > last_time_ && entry.start <= now)
                            cube_sensed_ = true ;
                    }

                    //
                    // Cubes based on position
                    //
                    double x = phoenix.getRobotXPos() ;
                    double y = phoenix.getRobotYPos() ;
                    if (phoenix.isCubeAtPosition(x, y)) {
                        phoenix.removeCube(x, y) ;
                        cube_sensed_ = true ;
                    }
                }

                if (input_ != nullptr)
                    input_->SimulatorSetValue(cube_sensed_) ;

                last_time_ = now ;


            }

            void CubeSensorModel::inputChanged(SimulatedObject *obj) {   
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr)
                    duty_ = victor->Get() ;
            }

            void CubeSensorModel::addDigitalInput(frc::DigitalInput *input) {
                if (input->GetChannel() == cube_sensor_input_) {
                    input_ = input ;
                    input_->SimulatorSetValue(false) ;
                    input_->addModel(this) ;
                }
            }

            void CubeSensorModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_1_) {
                    motor1_ = motor ;
                    motor1_->addModel(this) ;
                }
                else if (motor->GetChannel() == motor_channel_2_) {
                    motor2_ = motor ;
                    motor2_->addModel(this) ;
                }
            }               
        }
    }
}

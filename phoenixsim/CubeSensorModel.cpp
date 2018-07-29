#include "CubeSensorModel.h"
#include <RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            CubeSensorModel::CubeSensorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "cubesensor") {
                cube_sensed_ = false ;
                cube_sensor_input_ = simbase.getSettingsParser().getInteger("hw:collector:cubesensor") ;
                inited_ = false ;
                last_time_ = 0.0 ;
                input_ = nullptr ;
            }

            CubeSensorModel::~CubeSensorModel() {                
            }

            std::string CubeSensorModel::toString() {
                std::string result("cube sensor: ") ;

                result += "cube " ;
                result += (cube_sensed_ ? "present" : "notpresent") ;

                return result ;
            }

            std::vector<std::string> CubeSensorModel::split(const std::string &line) {
                size_t index = 0 ;
                std::string word ;
                std::vector<std::string> result ;

                while (index < line.length()) {
                    if (line[index] == ',') {
                        result.push_back(word) ;
                        word.clear() ;
                        index++ ;
                    }
                    else {
                        word += line[index++] ;
                    }
                }

                if (word.length() > 0)
                    result.push_back(word) ;

                return result ;
            }

            void CubeSensorModel::initModel() {
                if (getSimulator().hasProperty("cube")) {
                    const std::string &prop = getSimulator().getProperty("cube") ;
                    std::vector<std::string> words = split(prop) ;
                    for(const std::string &word: words) {
                        double t = std::stod(word) ;
                        OnTime tm(t) ;
                        ontimes_.push_back(tm) ;
                    }
                }
            }

            void CubeSensorModel::run(double dt) {
                if (!inited_) {
                    initModel() ;
                    inited_ = true ;
                }

                double now = getSimulator().getTime() ;

                if (cube_sensed_ == false) {
                    for(const auto &entry: ontimes_) {
                        if (entry.start > last_time_ && entry.start <= now)
                            cube_sensed_ = true ;
                    }
                }

                if (input_ != nullptr)
                    input_->SimulatorSetValue(cube_sensed_) ;

                last_time_ = now ;
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

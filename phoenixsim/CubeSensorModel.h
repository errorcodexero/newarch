#pragma once

#include <DigitalInput.h>
#include <SubsystemModel.h>
#include <vector>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class CubeSensorModel : public SubsystemModel {
            public:
                CubeSensorModel(RobotSimBase &simbase) ;
                virtual ~CubeSensorModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addDigitalInput(frc::DigitalInput *input) ;

                bool getCubeSensed() {
                    return cube_sensed_ ;
                }

            private:
                struct OnTime {
                    OnTime(double s) {
                        start = s ;
                    }
                    double start ;
                } ;

            private:
                void initModel() ;
                std::vector<std::string> split(const std::string &line) ;

            private:       
                frc::DigitalInput *input_ ;
                bool inited_ ;
                bool cube_sensed_ ;
                int cube_sensor_input_ ;
                double last_time_ ;

                std::list<OnTime> ontimes_ ;
            } ;
        }
    }
}

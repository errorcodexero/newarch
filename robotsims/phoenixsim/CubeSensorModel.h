#pragma once

#include <frc/DigitalInput.h>
#include <frc/SubsystemModel.h>
#include <vector>

namespace xero  {
    namespace sim {
        namespace phoenix {
            /// \brief A model of Phoenix's cube sensor.
            class CubeSensorModel : public SubsystemModel {
            public:
                /// \brief Create a new model of the robot's cube sensor.
                /// \param simbase a reference to the base for the robot simulator
                CubeSensorModel(RobotSimBase &simbase) ;

                /// \brief Destroy the model.
                virtual ~CubeSensorModel() ;

                virtual bool processEvent(const std::string &name, int value) ;                     

                /// \brief Convert the model to a string.
                /// \returns details about the cube sensor's status
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt) ;

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj) ;

                /// \brief Add a digital input to the model.
                /// \param input the digital input to all
                virtual void addDevice(frc::DigitalInput *input) ;

                /// \brief Add a Victor SP to the model.
                /// \param victor the Victor SP to add
                virtual void addDevice(frc::VictorSP *victor) ;                

                /// \brief Get whether a cube is sensed by the cube sensor.
                /// \returns true if a cube is sensed by the cube sensor
                bool getCubeSensed() {
                    return cube_sensed_ ;
                }

            private:
                void evalSensor() ;
                std::vector<std::string> split(const std::string &line) ;

            private:       
                frc::DigitalInput *input_ ;
                bool inited_ ;
                bool cube_sensed_ ;
                int cube_sensor_input_ ;
                double last_time_ ;

                frc::VictorSP *motor1_ ;
                frc::VictorSP *motor2_ ;   

                int motor_channel_1_ ;
                int motor_channel_2_ ;

                double duty_ ;
                bool running_ ;
                double start_ ;
                double eject_duration_ ;
            } ;
        }
    }
}

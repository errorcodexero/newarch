#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class LineSensorModel : public SubsystemModel {
            public:
                LineSensorModel(xero::sim::RobotSimBase &simbase, const std::string &name) ;

                virtual ~LineSensorModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                virtual void addDevice(frc::DigitalInput *) ;

                virtual void inputChanged(SimulatedObject *obj) {                    
                }

                virtual void processEvent(const std::string &event, int value) ;

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::DigitalInput *i0_ ;
                frc::DigitalInput *i1_ ;
                frc::DigitalInput *i2_ ;

                int i0num_ ;
                int i1num_ ;
                int i2num_ ;

                bool i0state_ ;
                bool i1state_ ;
                bool i2state_ ;

                double last_time_ ;
                std::vector<double> ontimes_ ;                   
            } ;
        }
    }
}
#pragma once

#include <frc/SubsystemModel.h>
#include <frc/DriverStation.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class OIModel : public SubsystemModel {
            public:
                OIModel(RobotSimBase &simbase) ;
                virtual ~OIModel() ;

                virtual void run(double dt) ;
                virtual std::string toString() ;
                virtual void init()  ;          

                virtual void addDevice(frc::DriverStation *station) ;
                virtual void inputChanged(SimulatedObject *obj) ;

                void setButton(int which, int button, bool value) ;
                void setAxis(int which, int button, double value) ;
                void setPOV(int which, int button, int value) ;

                bool getButton(int which, int button) ;
                double getAxis(int which, int axis) ;
                int getPOV(int which, int pov) ;

                frc::DriverStation *getDS() { 
                    if (ds_ == nullptr)
                        ds_ = &frc::DriverStation::GetInstance() ;
                        
                    return ds_ ;
                }

            private:
                frc::DriverStation *ds_ ;
                double autovalue_ ;
            } ;
        }
    }
}

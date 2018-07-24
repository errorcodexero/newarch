#pragma once

#include "SimulatedObject.h"

namespace frc {
    class DigitalInput : public xero::sim::SimulatedObject {
        friend class xero::sim::RobotSimBase ;
    public:
        DigitalInput(int which) ;
        virtual ~DigitalInput() ;

        bool Get()  ;

        int GetChannel() const {
            return which_ ;
        }

        void SimulatorSetValue(bool v) ;

    private:
        int which_ ;
        bool value_ ;
    } ;
}

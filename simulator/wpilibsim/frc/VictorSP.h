#pragma once

#include "SimulatedObject.h"

namespace frc {
    class VictorSP : public xero::sim::SimulatedObject {
    public:
        VictorSP(int which)  ;

        virtual ~VictorSP() ;

        void Set(double p)  ;

        double Get() const {
            return value_ ;
        }

        int GetChannel() const {
            return which_ ;
        }

        void SetInverted(bool b) {
            inverted_ = b ;
        }

    private:
        int which_ ;
        bool inverted_ ;
        double value_ ;
    } ;
}
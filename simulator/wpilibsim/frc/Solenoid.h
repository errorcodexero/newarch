#pragma once
#include "SimulatedObject.h"
#include <iostream>

namespace frc {
    class Solenoid : public xero::sim::SimulatedObject {
    public:      
        Solenoid(int channel) ;
        virtual ~Solenoid() ;

        virtual void Set(bool v)  ;

        virtual bool Get() const {
            return state_ ;
        }

        bool IsBlackListed() const {
            return blacklisted_ ;
        }

        void SetPulseDuration(double dur) {
            duration_ = dur ;
        }

        void StartPulse() {
        }

        int SimulatorGetChannel() const {
            return channel_ ;
        }

    private:
        int channel_ ;
        bool state_ ;
        bool blacklisted_ ;
        double duration_ ;
    } ;
}
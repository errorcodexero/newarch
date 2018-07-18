#pragma once
#include <iostream>

namespace frc {
    class Solenoid {
    public:      
        Solenoid(int channel) ;
        virtual ~Solenoid() ;

        virtual void Set(bool v) {
            state_ = v ;
            std::cout << "Solenoid " << channel_ ;
            std::cout << ": set to state " << (v ? "on" : "off") ; 
            std::cout << std::endl ;
        }

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

    private:
        int channel_ ;
        bool state_ ;
        bool blacklisted_ ;
        double duration_ ;
    } ;
}
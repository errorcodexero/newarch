#pragma once

#include <frc/Joystick.h>

namespace frc {

    class XboxController : public Joystick {
    public:
        enum RumbleType 
        { 
            kLeftRumble, 
            kRightRumble
        } ;

    public:
        XboxController(int port) : Joystick(port) {
        }

        virtual ~XboxController() {
        }

        void SetOutput(int output, bool state) {
        }

        void SetRumble(RumbleType rtype, double value) {            
        }
        
    } ;
}
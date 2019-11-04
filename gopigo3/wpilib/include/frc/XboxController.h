#pragma once

#include <frc/GenericHID.h>

namespace frc
{
    class XboxController : public GenericHID
    {
    public:
        XboxController(int index) : GenericHID(index) {
        }

        virtual ~XboxController() {            
        }

        double GetX(JoystickHand hand) ;
        double GetY(JoystickHand hand) ;
    } ;
}
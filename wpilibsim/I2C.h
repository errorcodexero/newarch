#pragma once

namespace frc
{
    class I2C {
    public:
        enum PortType 
        {
            kOnboard,
        } ;

    public:
        I2C(PortType p, int index) {            
        }

        virtual ~I2C() {            
        }
    } ;
}

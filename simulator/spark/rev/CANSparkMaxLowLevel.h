#pragma once

namespace rev
{
    class CANSparkMaxLowLevel
    {
    public:
        CANSparkMaxLowLevel() ;
        virtual ~CANSparkMaxLowLevel() ;

        enum MotorType
        {
            kBrushed,
            kBrushless,
        } ;
    } ;
}

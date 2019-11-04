#pragma once

namespace rev
{
    class CANSparkMaxLowLevel
    {
    public:
        enum MotorType { 
            kBrushed = 0, 
            kBrushless = 1 
        } ;

        enum ParameterStatus {
            kOK = 0, 
            kInvalidID = 1, 
            kMismatchType = 2, 
            kAccessMode = 3,
            kInvalid = 4, 
            kNotImplementedDeprecated = 5
        } ;
 
        enum PeriodicFrame { 
            kStatus0 = 0, 
            kStatus1 = 1, 
            kStatus2 = 2 
        } ;

    public:
        CANSparkMaxLowLevel(int devid, MotorType type) {
            id_ = devid ;
            type_ = type ;
        }

        virtual ~CANSparkMaxLowLevel() {            
        }

        void Set(double value) {
            power_ = value ;
        }
        double Get() const {
            return power_ ;
        }

        void EnableVoltageCompensation(double volts) {            
        }

    private:
        int id_ ;
        MotorType type_ ;
        double power_ ;
    } ;
}
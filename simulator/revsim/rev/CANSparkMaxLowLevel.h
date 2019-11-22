#pragma once

#include <frc/SimulatedObject.h>

namespace rev
{
    class CANSparkMaxLowLevel : public xero::sim::SimulatedObject
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
            if (value > 1.0)
                value = 1.0 ;
            else if (value < -1.0)
                value = -1.0 ;
                
            power_ = value ;
            changed() ;            
        }

        double Get() const {
            return power_ ;
        }

        void EnableVoltageCompensation(double volts) {            
        }

        int GetDeviceID() {
            return id_ ;
        }

        void SetInverted(bool b) {
            inverted_ = b ;
        }        
        
        bool GetInverted() const {
            return inverted_ ;
        }        

        double SimulatorGetPower() {
            if (inverted_)
                return -power_ ;
            else
                return power_ ;
        }

    private:
        int id_ ;
        bool inverted_ ;        
        MotorType type_ ;
        double power_ ;
    } ;
}

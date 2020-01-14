#pragma once

#include "CANEncoder.h"


namespace rev
{
    class CANEncoder ;

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
            encoder_ = new CANEncoder(*this);
        }

        virtual ~CANSparkMaxLowLevel() {
            delete encoder_;
        }

        CANEncoder &GetEncoder() {
            return *encoder_;
        }

        void Set(double value) {
            if (value > 1.0)
                value = 1.0 ;
            else if (value < -1.0)
                value = -1.0 ;
                
            power_ = value ;
        }

        double Get() const {
            return power_ ;
        }

        void EnableVoltageCompensation(double volts) {            
        }

        void SetSmartCurrentLimit(double current) {            
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
        CANEncoder *encoder_;
    } ;
}

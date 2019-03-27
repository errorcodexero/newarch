#pragma once

#include <frc/SimulatedObject.h>
#include <frc/SPI.h>
#include <string>

class AHRS : public xero::sim::SimulatedObject
{
public:
    AHRS(const char *port_p);
    AHRS(frc::SPI::Port p) ;
    virtual ~AHRS();

    bool IsConnected()
    {
        return true;
    }

    bool IsCalibrating() {
        return false ;
    }

    double GetYaw();
    void ZeroYaw();
    double GetAngle() {
        return 0.0 ;
    }

    double GetVelocityX() {
        return 0.0 ;
    }

    double GetVelocityY() {
        return 0.0 ;
    }

    double GetVelocityZ() {
        return 0.0 ;
    }

    void Reset() {
        yaw_ = 0.0 ;
    }

    void SimulatorSetYaw(double yaw) {
        yaw_ = yaw ;
    }

private:
    double yaw_;
};


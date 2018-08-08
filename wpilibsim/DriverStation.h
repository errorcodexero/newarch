#pragma once

#include "SimulatedObject.h"
#include <iostream>

namespace frc
{
    class DriverStation : public xero::sim::SimulatedObject {
        friend class xero::sim::RobotSimBase ;
    public:
        static DriverStation &GetInstance() {
            if (theOne == nullptr)
                theOne = new DriverStation() ;

            return *theOne ;
        }

        int GetStickAxisCount(int which) ;
        double GetStickAxis(int which, int i) ;
        int GetStickButtonCount(int which) ;
        bool GetStickButton(int which, int i) ;
        int GetStickPOVCount(int which) ;
        int GetStickPOV(int which, int i) ;
		void ReportError(const char *msg) ;

    private:
        DriverStation() ;

    private:
        static DriverStation *theOne ;
    } ;
}

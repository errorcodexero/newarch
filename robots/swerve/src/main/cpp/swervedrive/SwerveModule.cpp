#include "SwerveModule.h"
#include <iostream>

namespace xero {
    namespace base
    {
        SwerveModule::SwerveModule(int rotate, int drive)
        {
            std::cout << "SwerveModule::SwerveModule " << rotate << " " << drive << " " << std::endl ;
            rotate_ = std::make_shared<rev::CANSparkMax>(rotate, rev::CANSparkMaxLowLevel::MotorType::kBrushless) ;
            drive_ = std::make_shared<rev::CANSparkMax>(drive, rev::CANSparkMaxLowLevel::MotorType::kBrushless) ;
        }

        SwerveModule::~SwerveModule()
        {
        }
    }
}
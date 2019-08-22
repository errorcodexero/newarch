#include "SwerveModule.h"

namespace xero {
    namespace base
    {
        SwerveModule::SwerveModule(int rotate, int drive)
        {
            rotate_ = std::make_shared<rev::CANSparkMax>(rotate, rev::CANSparkMaxLowLevel::MotorType::kBrushless) ;
            drive_ = std::make_shared<rev::CANSparkMax>(drive, rev::CANSparkMaxLowLevel::MotorType::kBrushless) ;
        }

        SwerveModule::~SwerveModule()
        {
        }
    }
}
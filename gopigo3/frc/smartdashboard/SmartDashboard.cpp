#include <smartdashboard/SmartDashboard.h>
#include <iostream>

namespace frc
{
    std::shared_ptr<nt::NetworkTable> SmartDashboard::table_;

    void SmartDashboard::init()
    {
        table_ = nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard");
    }

    void SmartDashboard::PutString(wpi::StringRef key, wpi::StringRef value)
    {
        std::cout << "SmartDashboard::PutString called";
        std::cout << ", key = '" << key << "'";
        std::cout << "< value = '" << value << "'";
        std::cout << std::endl;
        init();
        table_->GetEntry(key).SetString(value);
    }

    void SmartDashboard::PutNumber(wpi::StringRef key, double value)
    {
        init();
        table_->GetEntry(key).SetDouble(value);
    }
}

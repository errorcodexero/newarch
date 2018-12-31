#include <smartdashboard/SmartDashboard.h>
#include <iostream>

namespace frc
{
    std::shared_ptr<nt::NetworkTable> SmartDashboard::table_;

    void SmartDashboard::init()
    {
        if (table_ == nullptr)
            table_ = nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard");
    }

    void SmartDashboard::PutString(wpi::StringRef key, wpi::StringRef value)
    {
        init();
        table_->GetEntry(key).SetString(value);
    }

    void SmartDashboard::PutNumber(wpi::StringRef key, double value)
    {
        init();
        table_->GetEntry(key).SetDouble(value);
    }

    void SmartDashboard::PutNumberArray(wpi::StringRef key, wpi::ArrayRef<double> value)
    {
        init();
        table_->GetEntry(key).SetDoubleArray(value);
    }

    void SmartDashboard::PutBoolean(wpi::StringRef key, bool value)
    {
        init();
        table_->GetEntry(key).SetBoolean(value);
    }
}

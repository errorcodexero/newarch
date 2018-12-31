#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <string>
#include <memory>

namespace frc {
	class SmartDashboard {
	public:
      static void PutString(wpi::StringRef key, wpi::StringRef value);
      static void PutNumber(wpi::StringRef key, double value);

    private:
      static void init();

    private:
        static std::shared_ptr<nt::NetworkTable> table_;
    } ;
}
#include "SwerveOIDevice.h"
#include "SwerveOISubsystem.h"
#include "SwerveRobot.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {

        SwerveOIDevice::SwerveOIDevice(SwerveOISubsystem &sub, int index) : OIDevice(sub, index) {
        }

        SwerveOIDevice::~SwerveOIDevice() {
        }
    }
}

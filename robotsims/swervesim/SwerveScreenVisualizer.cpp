#include "SwerveScreenVisualizer.h"
#include "SwerveSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace swerve {
            SwerveScreenVisualizer::SwerveScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            SwerveScreenVisualizer::~SwerveScreenVisualizer() {
            }

            void SwerveScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

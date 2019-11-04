#include "PhaserScreenVisualizer.h"
#include "PhaserSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace phaser {
            PhaserScreenVisualizer::PhaserScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            PhaserScreenVisualizer::~PhaserScreenVisualizer() {
            }

            void PhaserScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

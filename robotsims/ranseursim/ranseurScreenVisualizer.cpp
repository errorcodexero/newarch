#include "ranseurScreenVisualizer.h"
#include "ranseurSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace ranseur {
            ranseurScreenVisualizer::ranseurScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            ranseurScreenVisualizer::~ranseurScreenVisualizer() {
            }

            void ranseurScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

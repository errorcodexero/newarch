#include "droidScreenVisualizer.h"
#include "droidSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace droid {
            droidScreenVisualizer::droidScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            droidScreenVisualizer::~droidScreenVisualizer() {
            }

            void droidScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

#include "Bunny2018ScreenVisualizer.h"
#include "Bunny2018Simulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace bunny2018 {
            Bunny2018ScreenVisualizer::Bunny2018ScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            Bunny2018ScreenVisualizer::~Bunny2018ScreenVisualizer() {
            }

            void Bunny2018ScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

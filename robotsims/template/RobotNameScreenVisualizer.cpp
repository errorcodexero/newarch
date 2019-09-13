#include "$$RobotName$$ScreenVisualizer.h"
#include "$$RobotName$$Simulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace $$robotname$$ {
            $$RobotName$$ScreenVisualizer::$$RobotName$$ScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            $$RobotName$$ScreenVisualizer::~$$RobotName$$ScreenVisualizer() {
            }

            void $$RobotName$$ScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

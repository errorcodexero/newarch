#include "MotorControllerScreenVisualizer.h"
#include "MotorControllerSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace motorcontroller {
            MotorControllerScreenVisualizer::MotorControllerScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            MotorControllerScreenVisualizer::~MotorControllerScreenVisualizer() {
            }

            void MotorControllerScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }
        }
    }
}

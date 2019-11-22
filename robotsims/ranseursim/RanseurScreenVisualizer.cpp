#include "RanseurScreenVisualizer.h"
#include "RanseurSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace ranseur {
            RanseurScreenVisualizer::RanseurScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
            }

            RanseurScreenVisualizer::~RanseurScreenVisualizer() {
            }

            void RanseurScreenVisualizer::drawGameSpecificField(WINDOW *win) {
                int sx1, sy1, sx2, sy2 ;

                physicalToScreen(27.0 * 12.0, 0.0, sx1, sy1) ;
                physicalToScreen(27.0 * 12.0, 27.0 * 12.0, sx2, sy2) ;

                for(int y = sy2 - 1 ; y < sy1 - 1 ; y++) {
                    mvwaddch(win, y, sx1, ACS_VLINE) ;
                }
            }

            void RanseurScreenVisualizer::plotRobot(double x, double y, double angle) {
                int rx, ry ;

                ScreenVisualizer::plotRobot(x, y, angle) ;
                physicalToScreen(x, y, rx, ry) ;

                auto bunnyarm = std::dynamic_pointer_cast<BunnyArmModel>(getSimulator().getModelByName("bunnyarm")) ;
                auto tubmanip = std::dynamic_pointer_cast<TubManipulatorModel>(getSimulator().getModelByName("tubmanipulator")) ;
                if (bunnyarm->isDeployed())
                    drawFilledScreenRectangle(field_window_, '|', rx , ry, 1, 1) ;
                else
                    drawFilledScreenRectangle(field_window_, '-', rx , ry, 1, 1) ;                

                if (tubmanip->hasTub())
                    drawFilledScreenRectangle(field_window_, '#', rx - 1, ry - 1, 1, 1) ;

                wrefresh(field_window_) ;
            }
        }
    }
}

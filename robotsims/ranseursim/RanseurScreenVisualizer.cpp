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

            char RanseurScreenVisualizer::getRobotChar() 
            {
                auto tubmanip = std::dynamic_pointer_cast<TubManipulatorModel>(getSimulator().getModelByName("tubmanipulator")) ;

                return tubmanip->hasTub() ? '#' : ' ' ;
            }

            void RanseurScreenVisualizer::plotRobot(int x, int y, double angle, int width, int length) {
                ScreenVisualizer::plotRobot(x, y, angle, width, length) ;

                auto bunnyarm = std::dynamic_pointer_cast<BunnyArmModel>(getSimulator().getModelByName("bunnyarm")) ;

                auto db = std::dynamic_pointer_cast<TankDriveModel>(getSimulator().getModelByName("tankdrive"));
                
                if (bunnyarm->isDeployed())
                {
                    drawFilledScreenRectangle(field_window_, '|', x + width / 2 , y + 1, 1, length - 2) ;
                }
                else
                {
                    drawFilledScreenRectangle(field_window_, '-', x , y + length / 2, width - 2, 1) ;
                }

                wrefresh(field_window_) ;
            }
        }
    }
}

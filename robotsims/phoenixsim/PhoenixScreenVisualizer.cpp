#include "PhoenixScreenVisualizer.h"
#include "PhoenixSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace phoenix {
            PhoenixScreenVisualizer::PhoenixScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
                cube_ = false ;
            }

            PhoenixScreenVisualizer::~PhoenixScreenVisualizer() {
            }

            void PhoenixScreenVisualizer::drawGameSpecificField(WINDOW *win) {
                const double edgeToSwitchHorizontal = 140.0 ;
                const double edgeToSwitchVertical = 85.25 ;
                const double switchWidth = 56.0 ;
                const double switchHeight = 336 - edgeToSwitchVertical - edgeToSwitchVertical ;
                const double switchToScale = 103.65 ;
                const double edgeToScaleVertical = 71.57 ;
                const double scaleWidth = 48.0 ;
                const double scaleHeight = 336 - edgeToScaleVertical - edgeToScaleVertical ;
                double x1, y1 ;

                x1 = edgeToSwitchHorizontal ;
                y1 = edgeToSwitchVertical ;
                drawFieldRectangle(win, x1, y1, switchWidth, switchHeight) ;

                x1 = edgeToSwitchHorizontal + switchWidth + switchToScale ;
                y1 = edgeToScaleVertical ;
                drawFieldRectangle(win, x1, y1, scaleWidth, scaleHeight) ;

                x1 = edgeToSwitchHorizontal + switchWidth + switchToScale + scaleWidth + switchToScale ;
                y1 = edgeToSwitchVertical ;
                drawFieldRectangle(win, x1, y1, switchWidth, switchHeight) ;

                drawCubes(win) ;            
            }

            void PhoenixScreenVisualizer::drawCubes(WINDOW *win) {
                auto &sim = getSimulator() ;
                PhoenixSimulator &phoenix = dynamic_cast<PhoenixSimulator &>(sim) ;

                for(const Cube &cube : phoenix.getCubes()) {
                    drawFieldRectangle(win, cube.getX() - CubeWidthHeight / 2.0, cube.getY() - CubeWidthHeight / 2.0, CubeWidthHeight, CubeWidthHeight) ;
                }
            }
        }
    }
}

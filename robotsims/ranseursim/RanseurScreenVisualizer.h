#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace ranseur {
            class RanseurScreenVisualizer : public ScreenVisualizer {
            public:
                RanseurScreenVisualizer(RobotSimBase &sim) ;
                virtual ~RanseurScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
                virtual void plotRobot(double x, double y, double angle) ;
            } ;
        }
    }
}

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
                virtual void plotRobot(int x, int y, double angle, int width, int length) ;

                virtual char getRobotChar() ;                
            } ;
        }
    }
}

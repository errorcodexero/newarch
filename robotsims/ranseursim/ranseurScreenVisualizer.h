#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace ranseur {
            class ranseurScreenVisualizer : public ScreenVisualizer {
            public:
                ranseurScreenVisualizer(RobotSimBase &sim) ;
                virtual ~ranseurScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

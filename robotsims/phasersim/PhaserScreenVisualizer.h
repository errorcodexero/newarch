#pragma once
#include <frc/ScreenVisualizer.h>
#include <frc/TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class PhaserScreenVisualizer : public ScreenVisualizer {
            public:
                PhaserScreenVisualizer(RobotSimBase &sim) ;
                virtual ~PhaserScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

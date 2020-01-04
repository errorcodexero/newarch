#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace droid {
            class droidScreenVisualizer : public ScreenVisualizer {
            public:
                droidScreenVisualizer(RobotSimBase &sim) ;
                virtual ~droidScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

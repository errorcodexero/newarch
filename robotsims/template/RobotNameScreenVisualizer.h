#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace $$robotname$$ {
            class $$RobotName$$ScreenVisualizer : public ScreenVisualizer {
            public:
                $$RobotName$$ScreenVisualizer(RobotSimBase &sim) ;
                virtual ~$$RobotName$$ScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

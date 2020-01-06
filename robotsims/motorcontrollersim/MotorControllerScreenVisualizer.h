#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace motorcontroller {
            class MotorControllerScreenVisualizer : public ScreenVisualizer {
            public:
                MotorControllerScreenVisualizer(RobotSimBase &sim) ;
                virtual ~MotorControllerScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

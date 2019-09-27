#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace swerve {
            class SwerveScreenVisualizer : public ScreenVisualizer {
            public:
                SwerveScreenVisualizer(RobotSimBase &sim) ;
                virtual ~SwerveScreenVisualizer() ;

            protected:
                virtual void drawGameSpecificField(WINDOW *win)  ;
            } ;
        }
    }
}

#pragma once
#include <frc/ScreenVisualizer.h>
#include <TankDriveModel.h>
#include "LifterModel.h"
#include "IntakeModel.h"
#include "GrabberModel.h"
#include "WingsModel.h"
#include "CubeSensorModel.h"
#include "OIModel.h"

#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace phoenix {
            class PhoenixScreenVisualizer : public ScreenVisualizer {
            public:
                PhoenixScreenVisualizer(RobotSimBase &sim) ;
                virtual ~PhoenixScreenVisualizer() ;

            protected:
                virtual char getRobotChar() {
                    return cube_ ? '#' : ' ' ;
                }

                virtual void drawGameSpecificField(WINDOW *win)  ;


            private:
                constexpr static double CubeWidthHeight = 11.0 ;

                void drawCubes(WINDOW *win) ;

            private:
                int cube_dim ;
                bool cube_ ;
            } ;
        }
    }
}

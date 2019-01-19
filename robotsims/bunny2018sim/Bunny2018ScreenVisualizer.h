#pragma once
#include <frc/ScreenVisualizer.h>
#include <frc/TankDriveModel.h>
#include "CollectorModel.h"
#include "HopperModel.h"
#include "SorterModel.h"
#include "OIModel.h"

#include <ncurses.h>

namespace xero {
    namespace sim {
        namespace bunny2018 {
            class Bunny2018ScreenVisualizer : public ScreenVisualizer {
            public:
                Bunny2018ScreenVisualizer(RobotSimBase &sim) ;
                virtual ~Bunny2018ScreenVisualizer() ;

            protected:
                virtual char getRobotChar() {
                    return ' ' ;
                }

                virtual void drawGameSpecificField(WINDOW *win)  ;
            private:
            } ;
        }
    }
}

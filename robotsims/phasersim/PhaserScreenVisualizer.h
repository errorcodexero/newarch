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

                virtual void beginCycle(double t) ;
                virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) ;

            protected:
                virtual char getRobotChar() {
                    return ' ' ;
                }

                virtual std::string getModeString() ;

                virtual void drawGameSpecificField(WINDOW *win)  ;


            private:
                void displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) ;

            private:
                int gamedata_row_ ;
                int tankdrive_row_ ;
            } ;
        }
    }
}

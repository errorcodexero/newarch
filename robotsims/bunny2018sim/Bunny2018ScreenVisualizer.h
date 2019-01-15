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

                virtual void beginCycle(double t) ;
                virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) ;

            protected:
                virtual char getRobotChar() {
                    return ' ' ;
                }

                virtual std::string getModeString() ;

                virtual void drawGameSpecificField(WINDOW *win)  ;


            private:
                constexpr static double CubeWidthHeight = 11.0 ;

                void displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) ;
                void displayOI(std::shared_ptr<OIModel> subsystem_p) ;
                void displayCollector(std::shared_ptr<xero::sim::bunny2018::CollectorModel> subsystem_p) ;
                void displayHopper(std::shared_ptr<xero::sim::bunny2018::HopperModel> subsystem_p) ;
                void displaySorter(std::shared_ptr<xero::sim::bunny2018::SorterModel> subsystem_p) ;

                int displayBaseOI(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;
                int displayDriver(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;
                int displayGunner(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;

            private:
                int tankdrive_row_ ;
                int collector_row_ ;
                int hopper_row_ ;
                int sorter_row_ ;
            } ;
        }
    }
}

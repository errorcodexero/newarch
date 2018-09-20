#pragma once
#include <ScreenVisualizer.h>
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

				virtual void beginCycle(double t) ;
                virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) ;

			protected:
				virtual char getRobotChar() {
					return cube_ ? '#' : ' ' ;
				}

				virtual std::string getModeString() ;

				virtual void drawGameSpecificField(WINDOW *win)  ;


            private:
				constexpr static double CubeWidthHeight = 11.0 ;

                void displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) ;
                void displayLifter(std::shared_ptr<LifterModel> subsystem_p) ;
                void displayGrabber(std::shared_ptr<GrabberModel> subsystem_p) ;
                void displayIntake(std::shared_ptr<IntakeModel> subsystem_p) ;
                void displayWings(std::shared_ptr<WingsModel> subsystem_p) ;
                void displayCubeSensor(std::shared_ptr<CubeSensorModel> subsystem_p) ;
				void displayOI(std::shared_ptr<OIModel> subsystem_p) ;

				int displayBaseOI(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;
				int displayDriver(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;
				int displayGunner(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) ;

				void drawCubes(WINDOW *win) ;

            private:
				int gamedata_row_ ;
				int tankdrive_row_ ;
				int lifter_row_ ;
				int intake_row_ ;
				int grabber_row_ ;
				int wings_row_ ;
				int cubesensor_row_ ;
				int cube_dim ;
				bool cube_ ;
            } ;
        }
    }
}

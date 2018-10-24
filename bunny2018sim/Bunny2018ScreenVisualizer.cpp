#include "Bunny2018ScreenVisualizer.h"
#include "SampleRobot.h"
#include "Bunny2018Simulator.h"
#include <xeromath.h>
#include <DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace bunny2018 {
            Bunny2018ScreenVisualizer::Bunny2018ScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {

				int row = getFirstSubsystemRow() ;
                tankdrive_row_ = row ;
            }

            Bunny2018ScreenVisualizer::~Bunny2018ScreenVisualizer() {
            }

			void Bunny2018ScreenVisualizer::drawGameSpecificField(WINDOW *win) {
			}

			void Bunny2018ScreenVisualizer::beginCycle(double dt) {
				ScreenVisualizer::beginCycle(dt) ;
			}

			std::string Bunny2018ScreenVisualizer::getModeString() {
				std::string str("Finished") ;

				if (getSimulator().getRobot()->IsDisabled())
					str = "Disabled" ;
				else if (getSimulator().getRobot()->IsAutonomous())
					str = "Autonomous" ;
				else if (getSimulator().getRobot()->IsOperatorControl())
					str = "Teleop" ;
				else if (getSimulator().getRobot()->IsTest())
					str = "Test" ;				

				return str ;
			}

            void Bunny2018ScreenVisualizer::visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) {
                if (subsystem_p->getName() == "tankdrive")
                    displayTankDrive(std::dynamic_pointer_cast<xero::sim::TankDriveModel>(subsystem_p)) ;
				else if (subsystem_p->getName() == "oi")
					displayOI(std::dynamic_pointer_cast<OIModel>(subsystem_p)) ;
            }

			void Bunny2018ScreenVisualizer::displayOI(std::shared_ptr<OIModel> subsystem_p) {
				int ch ;
				int oi = 2 ;

				WINDOW *win = getOIWindow() ;
				int width = getOIWindowWidth() ;
				int top = 0 ;

				top = displayBaseOI(win, subsystem_p, 2, top, width) ;
				top = displayDriver(win, subsystem_p, 0, top, width) ;
				top = displayGunner(win, subsystem_p, 1, top, width) ;

				ch = wgetch(win) ;
			}

			int Bunny2018ScreenVisualizer::displayBaseOI(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
				std::string str ;

				wmove(win, top++, 0) ;
                wclrtoeol(win) ;

				top++ ;
				return top ;
			
			}

			int Bunny2018ScreenVisualizer::displayDriver(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
				return top ;				
			}

			int Bunny2018ScreenVisualizer::displayGunner(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
				return top ;
			}

            void Bunny2018ScreenVisualizer::displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),tankdrive_row_, 0 ) ;
                waddstr(getRobotWindow(),"Tank Drive") ;

                wmove(getRobotWindow(),tankdrive_row_ + 1, 0 ) ;
                str = "  X: " ;
                str += std::to_string(subsystem_p->getXPos()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),tankdrive_row_ + 2, 0 ) ;
                str = "  Y: " ;
                str += std::to_string(subsystem_p->getYPos()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                

                wmove(getRobotWindow(),tankdrive_row_ + 3, 0 ) ;
                str = "  Angle: " ;
                str += std::to_string(xero::math::rad2deg(subsystem_p->getAngle())) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),tankdrive_row_ + 4, 0 ) ;
                str = "  Speed: " ;
                str += std::to_string(subsystem_p->getSpeed()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;    

                wmove(getRobotWindow(),tankdrive_row_ + 5, 0 ) ;
                str = "  Max Speed: " ;
                str += std::to_string(subsystem_p->getMaxSpeed()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                              

            }
        }
    }
}

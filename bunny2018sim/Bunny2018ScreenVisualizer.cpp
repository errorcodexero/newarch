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

				hopper_row_ = row + 8 ;
				collector_row_ = row + 9 ;
				sorter_row_ = row + 11 ;
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
				else if (subsystem_p->getName() == "collector")
					displayCollector(std::dynamic_pointer_cast<CollectorModel>(subsystem_p)) ;
				else if (subsystem_p->getName() == "hopper")
					displayHopper(std::dynamic_pointer_cast<HopperModel>(subsystem_p)) ;
				else if (subsystem_p->getName() == "sorter")
					displaySorter(std::dynamic_pointer_cast<SorterModel>(subsystem_p)) ;										
            }

			void Bunny2018ScreenVisualizer::displayOI(std::shared_ptr<OIModel> subsystem_p) {
				WINDOW *win = getOIWindow() ;
				int width = getOIWindowWidth() ;
				int top = 0 ;

				top = displayBaseOI(win, subsystem_p, 2, top, width) ;
				top = displayDriver(win, subsystem_p, 0, top, width) ;
				top = displayGunner(win, subsystem_p, 1, top, width) ;
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

            void Bunny2018ScreenVisualizer::displayCollector(std::shared_ptr<xero::sim::bunny2018::CollectorModel> subsystem_p) {
                wmove(getRobotWindow(),collector_row_, 0 ) ;
				std::string text = "Collector: " + std::to_string(subsystem_p->getVoltage()) ;
                waddstr(getRobotWindow(), text.c_str()) ;				
			}

            void Bunny2018ScreenVisualizer::displayHopper(std::shared_ptr<xero::sim::bunny2018::HopperModel> subsystem_p) {
                wmove(getRobotWindow(),hopper_row_, 0 ) ;
				std::string text = "Hopper: " + std::to_string(subsystem_p->getVoltage()) ;
                waddstr(getRobotWindow(), text.c_str()) ;
			}

            void Bunny2018ScreenVisualizer::displaySorter(std::shared_ptr<xero::sim::bunny2018::SorterModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),sorter_row_, 0 ) ;
				std::string text = "Sorter" ;	
                waddstr(getRobotWindow(), text.c_str()) ;

                wmove(getRobotWindow(),sorter_row_ + 1, 0 ) ;
				text = "Input Motor: " + std::to_string(subsystem_p->getInVoltage()) ;
				text = text.substr(0, fieldwidth) ;
                waddstr(getRobotWindow(), text.c_str()) ;				

                wmove(getRobotWindow(),sorter_row_ + 2, 0 ) ;
				text = "Sorter Motor: " + std::to_string(subsystem_p->getSorterVoltage()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;	

                wmove(getRobotWindow(),sorter_row_ + 3, 0 ) ;
				text = "Output Motor: " + std::to_string(subsystem_p->getOutVoltage()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;	

                wmove(getRobotWindow(),sorter_row_ + 4, 0 ) ;
				text = "Wheel Angle: " + std::to_string(subsystem_p->getAngle()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;

                wmove(getRobotWindow(),sorter_row_ + 5, 0 ) ;
				text = "Index: " + std::to_string(subsystem_p->getIndexValue()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;		

                wmove(getRobotWindow(),sorter_row_ + 6, 0 ) ;
				text = "Ball Present: " + std::to_string(subsystem_p->getBallPresentState()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;

                wmove(getRobotWindow(),sorter_row_ + 7, 0 ) ;
				text = "Red Blue: " + std::to_string(subsystem_p->getBallRedState()) ;
				text = text.substr(0, fieldwidth) ;				
                waddstr(getRobotWindow(), text.c_str()) ;										
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

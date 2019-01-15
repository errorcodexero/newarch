#include "PhoenixScreenVisualizer.h"
#include "PhoenixSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace phoenix {
            PhoenixScreenVisualizer::PhoenixScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {

                cube_ = false ;

                int row = getFirstSubsystemRow() ;
                gamedata_row_ = row ;
                tankdrive_row_ = row + 2;
                lifter_row_ = row + 9 ;
                intake_row_ = row + 17 ;
                grabber_row_ = row + 21 ;
                wings_row_ = row + 25;
                cubesensor_row_ = row + 28 ;
            }

            PhoenixScreenVisualizer::~PhoenixScreenVisualizer() {
            }

            void PhoenixScreenVisualizer::drawGameSpecificField(WINDOW *win) {
                const double edgeToSwitchHorizontal = 140.0 ;
                const double edgeToSwitchVertical = 85.25 ;
                const double switchWidth = 56.0 ;
                const double switchHeight = 336 - edgeToSwitchVertical - edgeToSwitchVertical ;
                const double switchToScale = 103.65 ;
                const double edgeToScaleVertical = 71.57 ;
                const double scaleWidth = 48.0 ;
                const double scaleHeight = 336 - edgeToScaleVertical - edgeToScaleVertical ;
                double x1, y1 ;

                x1 = edgeToSwitchHorizontal ;
                y1 = edgeToSwitchVertical ;
                drawFieldRectangle(win, x1, y1, switchWidth, switchHeight) ;

                x1 = edgeToSwitchHorizontal + switchWidth + switchToScale ;
                y1 = edgeToScaleVertical ;
                drawFieldRectangle(win, x1, y1, scaleWidth, scaleHeight) ;

                x1 = edgeToSwitchHorizontal + switchWidth + switchToScale + scaleWidth + switchToScale ;
                y1 = edgeToSwitchVertical ;
                drawFieldRectangle(win, x1, y1, switchWidth, switchHeight) ;

                drawCubes(win) ;            
            }

            void PhoenixScreenVisualizer::beginCycle(double dt) {
                ScreenVisualizer::beginCycle(dt) ;

                std::string data("GameData: ") ;

                if (getSimulator().hasProperty("gamedata"))
                    data += getSimulator().getProperty("gamedata") ;
                else
                    data += "???" ;

                wmove(getRobotWindow(), gamedata_row_, 0) ;
                waddstr(getRobotWindow(), data.c_str()) ;
            }

            std::string PhoenixScreenVisualizer::getModeString() {
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

            void PhoenixScreenVisualizer::visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) {
                if (subsystem_p->getName() == "lifter")
                    displayLifter(std::dynamic_pointer_cast<LifterModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "intake")
                    displayIntake(std::dynamic_pointer_cast<IntakeModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "tankdrive")
                    displayTankDrive(std::dynamic_pointer_cast<xero::sim::TankDriveModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "wings")
                    displayWings(std::dynamic_pointer_cast<WingsModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "grabber")
                    displayGrabber(std::dynamic_pointer_cast<GrabberModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "cubesensor")
                    displayCubeSensor(std::dynamic_pointer_cast<CubeSensorModel>(subsystem_p)) ;
                else if (subsystem_p->getName() == "oi")
                    displayOI(std::dynamic_pointer_cast<OIModel>(subsystem_p)) ;
            }

            void PhoenixScreenVisualizer::displayOI(std::shared_ptr<OIModel> subsystem_p) {
                int ch ;
                int oi = 2 ;

                WINDOW *win = getOIWindow() ;
                int width = getOIWindowWidth() ;
                int top = 0 ;

                top = displayBaseOI(win, subsystem_p, 2, top, width) ;
                top = displayDriver(win, subsystem_p, 0, top, width) ;
                top = displayGunner(win, subsystem_p, 1, top, width) ;

                ch = wgetch(win) ;
                if (ch == 'f')
                    subsystem_p->setButton(oi, 1, !subsystem_p->getButton(oi, 1)) ;
                else if (ch == 'x')
                    subsystem_p->setButton(oi, 2, !subsystem_p->getButton(oi, 2)) ;
                else if (ch == 's')
                    subsystem_p->setButton(oi, 4, !subsystem_p->getButton(oi, 4)) ;             
                else if (ch == 'w')
                    subsystem_p->setButton(oi, 3, !subsystem_p->getButton(oi, 3)) ;
                else if (ch == 'c')
                    subsystem_p->setButton(oi, 8, !subsystem_p->getButton(oi, 8)) ;     
                else if (ch == 'C')
                    subsystem_p->setButton(oi, 5, !subsystem_p->getButton(oi, 5)) ; 
                else if (ch == 'E')
                    subsystem_p->setButton(oi, 6, !subsystem_p->getButton(oi, 6)) ;
                else if (ch == 'D')
                    subsystem_p->setButton(oi, 7, !subsystem_p->getButton(oi, 7)) ; 
                else if (ch == 'L')
                    subsystem_p->setButton(oi, 15, !subsystem_p->getButton(oi, 15)) ;
                else if (ch == 'W')
                    subsystem_p->setButton(oi, 9, !subsystem_p->getButton(oi, 9)) ; 
                else if (ch == 'S')
                    subsystem_p->setButton(oi, 10, !subsystem_p->getButton(oi, 10)) ;
                else if (ch == 'u')
                    subsystem_p->setButton(oi, 13, !subsystem_p->getButton(oi, 13)) ;
                else if (ch == 'd')
                    subsystem_p->setButton(oi, 14, !subsystem_p->getButton(oi, 14)) ;
                else if (ch == 'p')
                    subsystem_p->setButton(oi, 16, !subsystem_p->getButton(oi, 16)) ;
                else if (ch == KEY_UP) {
                    if (subsystem_p->getPOV(1, 0) == 0)
                        subsystem_p->setPOV(1, 0, -1) ;
                    else
                        subsystem_p->setPOV(1, 0, 0) ;
                } else if (ch == KEY_DOWN) {
                    if (subsystem_p->getPOV(1, 0) == 180)
                        subsystem_p->setPOV(1, 0, -1) ;
                    else
                        subsystem_p->setPOV(1, 0, 180) ;
                } else if (ch == KEY_RIGHT) {
                    if (subsystem_p->getPOV(1, 0) == 90)
                        subsystem_p->setPOV(1, 0, -1) ;
                    else
                        subsystem_p->setPOV(1, 0, 90) ;
                } else if (ch == KEY_LEFT) {
                    if (subsystem_p->getPOV(1, 0) == 270)
                        subsystem_p->setPOV(1, 0, -1) ;
                    else
                        subsystem_p->setPOV(1, 0, 270) ;
                }
            }

            int PhoenixScreenVisualizer::displayBaseOI(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
                std::string str ;

                wmove(win, top++, 0) ;
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;  
                str = "Floor[f]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(0) ?  "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;
                str = "Exchange[x]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(1) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;
                str = "Switch[w]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(2) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;
                str = "Scale[s]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(3) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;                                    

                wmove(win, top++, 0) ;
                str = "Climb[c]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(7) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;
                top++ ;     

                wmove(win, top++, 0) ;
                str = "Collect[C]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(4) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;
                str = "Eject[E]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(5) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;

                wmove(win, top++, 0) ;
                str = "Drop[D]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(6) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;    

                wmove(win, top++, 0) ;
                str = "EjectSlow[S]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(9) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;    

                top++ ;
                wmove(win, top++, 0) ;
                str = "LifterUp[u]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(12) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win)  ;

                wmove(win, top++, 0) ;
                str = "Lifter Down[d]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(13) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win)  ;

                wmove(win, top++, 0) ;
                str = "Lifter Power[p]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(15) ? "high" : "low") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win)  ;                                                           

                top++ ; 
                wmove(win, top++, 0) ;
                str = "ClimbDisabled[L]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(14) ? "true" : "false") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;        

                wmove(win, top++, 0) ;
                str = "Wings[W]: " ;
                str += (subsystem_p->getDS()->getStick(stick).getButtonValue(8) ? "deploy" : "nop") ;
                waddstr(win, str.substr(0, width).c_str()) ;                
                wclrtoeol(win) ;

            
                top++ ;
                return top ;
            
            }

            int PhoenixScreenVisualizer::displayDriver(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
                std::string str ;

                wmove(win, top++, 0) ;
                str = "POV[Arrows]: " ;
                str += std::to_string(subsystem_p->getDS()->getStick(stick).getPOVValue(0)) ;
                waddstr(win, str.substr(0, width).c_str()) ;
                wclrtoeol(win) ;    

                return top ;                
            }

            int PhoenixScreenVisualizer::displayGunner(WINDOW *win, std::shared_ptr<OIModel> subsystem_p, int stick, int top, int width) {
                return top ;
            }

            void PhoenixScreenVisualizer::displayLifter(std::shared_ptr<LifterModel> subsystem_p) {
                int fieldwidth = getRobotWindowWidth() ;
                std::string str ;

                wmove(getRobotWindow(),lifter_row_, 0) ;
                waddstr(getRobotWindow(),"Lifter") ;

                wmove(getRobotWindow(),lifter_row_ + 1, 0 ) ;
                str = "  Height: " ;
                str += std::to_string(subsystem_p->getHeight()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),lifter_row_ + 2, 0 ) ;
                str = "  Motor: " ;
                str += std::to_string(subsystem_p->getMotor()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ; 

                wmove(getRobotWindow(),lifter_row_ + 3, 0 ) ;
                str = "  Brake: " ;
                str += (subsystem_p->getBrake() ? "ON" : "OFF") ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;       

                wmove(getRobotWindow(),lifter_row_ + 4, 0 ) ;
                str = "  Gear: " ;
                str += (subsystem_p->getHighGear() ? "HIGH" : "LOW") ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),lifter_row_ + 5, 0 ) ;
                str = "  Top: " ;
                str += (subsystem_p->getTopLimit() ? "ON" : "OFF") ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),lifter_row_ + 6, 0 ) ;
                str = "  Bottom: " ;
                str += (subsystem_p->getBottomLimit() ? "ON" : "OFF") ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;
            }

            void PhoenixScreenVisualizer::displayIntake(std::shared_ptr<IntakeModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),intake_row_, 0 ) ;
                waddstr(getRobotWindow(),"Intake") ;

                wmove(getRobotWindow(),intake_row_ + 1, 0 ) ;
                str = "  Left: " + std::to_string(subsystem_p->getVoltage1()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),intake_row_ + 2, 0 ) ;
                str = "  Right: " + std::to_string(subsystem_p->getVoltage2()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                                
            }

            void PhoenixScreenVisualizer::displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) {
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
            }

            void PhoenixScreenVisualizer::displayWings(std::shared_ptr<WingsModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),wings_row_, 0 ) ;
                waddstr(getRobotWindow(),"Wings") ;

                wmove(getRobotWindow(),wings_row_ + 1, 0 ) ;
                str = "  Deployed: " ;
                str += (subsystem_p->getWings() ? "YES" : "NO") ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                  
            }

            void PhoenixScreenVisualizer::displayGrabber(std::shared_ptr<GrabberModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),grabber_row_, 0 ) ;
                waddstr(getRobotWindow(),"Grabber") ;

                wmove(getRobotWindow(),grabber_row_ + 1, 0 ) ;
                str = "  Angle: " + std::to_string(subsystem_p->getAngle()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;

                wmove(getRobotWindow(),grabber_row_ + 2, 0 ) ;
                str = "  Motor: " + std::to_string(subsystem_p->getVoltage()) ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                               
            }

            void PhoenixScreenVisualizer::displayCubeSensor(std::shared_ptr<CubeSensorModel> subsystem_p) {
                std::string str ;
                int fieldwidth = getRobotWindowWidth() ;

                wmove(getRobotWindow(),cubesensor_row_, 0 ) ;
                waddstr(getRobotWindow(),"Cube Sensor") ;

                cube_ = subsystem_p->getCubeSensed() ;

                wmove(getRobotWindow(),cubesensor_row_ + 1, 0 ) ;
                str = "  Present: " ;
                if (cube_)
                    str += "YES" ;
                else
                    str += "NO" ;
                waddstr(getRobotWindow(),str.substr(0, fieldwidth).c_str()) ;
                wclrtoeol(getRobotWindow()) ;                   
            }

            void PhoenixScreenVisualizer::drawCubes(WINDOW *win) {
                auto &sim = getSimulator() ;
                PhoenixSimulator &phoenix = dynamic_cast<PhoenixSimulator &>(sim) ;

                for(const Cube &cube : phoenix.getCubes()) {
                    drawFieldRectangle(win, cube.getX() - CubeWidthHeight / 2.0, cube.getY() - CubeWidthHeight / 2.0, CubeWidthHeight, CubeWidthHeight) ;
                }
            }
        }
    }
}

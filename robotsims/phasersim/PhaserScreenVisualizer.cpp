#include "PhaserScreenVisualizer.h"
#include "PhaserSimulator.h"
#include <xeromath.h>
#include <frc/SampleRobot.h>
#include <frc/DriverStation.h>
#include <cassert>
#include <csignal>
#include <memory>

namespace xero {
    namespace sim {
        namespace phaser {
            PhaserScreenVisualizer::PhaserScreenVisualizer(RobotSimBase &sim) : ScreenVisualizer(sim, "tankdrive") {
                int row = getFirstSubsystemRow() ;
                gamedata_row_ = row ;
                tankdrive_row_ = row + 2;
            }

            PhaserScreenVisualizer::~PhaserScreenVisualizer() {
            }

            void PhaserScreenVisualizer::drawGameSpecificField(WINDOW *win) {
            }

            void PhaserScreenVisualizer::beginCycle(double dt) {
                ScreenVisualizer::beginCycle(dt) ;

                std::string data("GameData: ") ;

                if (getSimulator().hasProperty("gamedata"))
                    data += getSimulator().getProperty("gamedata") ;
                else
                    data += "???" ;

                wmove(getRobotWindow(), gamedata_row_, 0) ;
                waddstr(getRobotWindow(), data.c_str()) ;
            }

            std::string PhaserScreenVisualizer::getModeString() {
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

            void PhaserScreenVisualizer::visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p) {
                if (subsystem_p->getName() == "tankdrive")
                    displayTankDrive(std::dynamic_pointer_cast<xero::sim::TankDriveModel>(subsystem_p)) ;
            }

            void PhaserScreenVisualizer::displayTankDrive(std::shared_ptr<xero::sim::TankDriveModel> subsystem_p) {
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
        }
    }
}

#include "CollectorEjectCubeAction.h"
#include "Collector.h"
#include "intake/Intake.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/Grabber.h"
#include "grabber/GrabberToAngleAction.h"
#include <Robot.h>

namespace xero {
    namespace phoenix {
        CollectorEjectCubeAction::CollectorEjectCubeAction(Collector &col, double speed) : CollectorAction(col) {
            speed_ = speed ;
            delay_ = 0.0 ;
            sensor_ = true ;
        }

        CollectorEjectCubeAction::CollectorEjectCubeAction(Collector &col, const std::string &speed) : CollectorAction(col) {
            speed_ = getCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = 0.0 ;
            sensor_ = true ;                
        }

        CollectorEjectCubeAction::CollectorEjectCubeAction(Collector &col, double speed, double delay) : CollectorAction(col) {
            speed_ = speed ;
            delay_ = delay ;
            sensor_ = false ;
        }

        CollectorEjectCubeAction::CollectorEjectCubeAction(Collector &col, const std::string &speed, const std::string &delay) : CollectorAction(col) {
            speed_ = getCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = getCollector().getRobot().getSettingsParser().getDouble(delay) ;
            sensor_ = false ;
        }

        void CollectorEjectCubeAction::start() {
            auto intake = getCollector().getIntake() ;
            auto intakeaction = std::make_shared<IntakeDutyCycleAction>(*intake, speed_) ;
            intake->setAction(intakeaction) ;

            auto grabber = getCollector().getGrabber() ;
            auto grabberaction = std::make_shared<GrabberToAngleAction>(*grabber, "grabber:angle:eject") ;
            grabber->setAction(grabberaction) ;

            start_ = getCollector().getRobot().getTime() ;
            isdone_ = false ;
        }

        void CollectorEjectCubeAction::run() {
            auto intake = getCollector().getIntake() ;            
            if (sensor_) {
                if (getCollector().hasCube() == false) {
                    auto intakeaction = std::make_shared<IntakeDutyCycleAction>(*intake, 0.0) ;
                    intake->setAction(intakeaction) ;
                    isdone_ = true ;
                }
            }
            else {
                double now = getCollector().getRobot().getTime() ;
                if (now > start_ + delay_) {
                    intake->setAction(nullptr) ;
                    isdone_ = true ;                    
                }
            }
        }

        bool CollectorEjectCubeAction::isDone() {
            return isdone_ ;
        }

        void CollectorEjectCubeAction::cancel() {
            auto intake = getCollector().getIntake() ;
            intake->setAction(nullptr) ;
            isdone_ = true ;            
        }

        std::string CollectorEjectCubeAction::toString() {
            std::string ret ;

            if (sensor_)
                ret = "EjectCube(sensor, " + std::to_string(speed_) + ")" ;
            else
                ret = "EjectCube(" + std::to_string(speed_) + "," + std::to_string(delay_) + ")" ;

            return ret ;
        }
    }
}
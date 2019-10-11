#include "CollectorEjectTubAction.h"
#include "Collector.h"
#include "Intake.h"
#include "IntakeDutyCycleAction.h"
#include <Robot.h>

namespace xero {
    namespace ranseur {
        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, double speed) : CollectorAction(col) {
            speed_ = speed ;
            delay_ = 0.0 ;
            sensor_ = true ;
        }

        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, const std::string &speed) : CollectorAction(col) {
            speed_ = getCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = 0.0 ;
            sensor_ = true ;                
        }

        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, double speed, double delay) : CollectorAction(col) {
            speed_ = speed ;
            delay_ = delay ;
            sensor_ = false ;
        }

        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, const std::string &speed, const std::string &delay) : CollectorAction(col) {
            speed_ = getCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = getCollector().getRobot().getSettingsParser().getDouble(delay) ;
            sensor_ = false ;
        }

        void CollectorEjectTubAction::start() {
            auto intake = getCollector().getIntake() ;
            auto intakeaction = std::make_shared<IntakeDutyCycleAction>(*intake, speed_) ;
            intake->setAction(intakeaction) ;

            start_ = getCollector().getRobot().getTime() ;
            isdone_ = false ;
        }

        void CollectorEjectTubAction::run() {
            auto intake = getCollector().getIntake() ;            
            if (sensor_) {
                if (getCollector().hasTub() == false) {
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

        bool CollectorEjectTubAction::isDone() {
            return isdone_ ;
        }

        void CollectorEjectTubAction::cancel() {
            auto intake = getCollector().getIntake() ;
            intake->setAction(nullptr) ;
            isdone_ = true ;            
        }

        std::string CollectorEjectTubAction::toString() {
            std::string ret ;

            if (sensor_)
                ret = "EjectTub(sensor, " + std::to_string(speed_) + ")" ;
            else
                ret = "EjectTub(" + std::to_string(speed_) + "," + std::to_string(delay_) + ")" ;

            return ret ;
        }
    }
}
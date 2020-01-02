#include "CollectorEjectCubeAction.h"
#include "Collector.h"
#include "grabber/Grabber.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

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
            CollectorAction::start();

            auto intake = getCollector().getIntake() ;
            auto intakeaction = std::make_shared<SingleMotorPowerAction>(*intake, speed_) ;
            intake->setAction(intakeaction, true) ;

            auto grabber = getCollector().getGrabber() ;
            auto grabberaction = std::make_shared<MotorEncoderGoToAction>(*grabber, "grabber:angle:eject") ;
            grabber->setAction(grabberaction, true) ;

            start_ = getCollector().getRobot().getTime() ;
        }

        void CollectorEjectCubeAction::run() {
            CollectorAction::run();

            auto intake = getCollector().getIntake() ;            
            if (sensor_) {
                if (getCollector().hasCube() == false) {
                    auto intakeaction = std::make_shared<SingleMotorPowerAction>(*intake, 0.0) ;
                    intake->setAction(intakeaction, true) ;
                    setDone();
                }
            }
            else {
                double now = getCollector().getRobot().getTime() ;
                if (now > start_ + delay_) {
                    auto intakeaction = std::make_shared<SingleMotorPowerAction>(*intake, 0.0) ;
                    intake->setAction(intakeaction, true) ;
                    setDone();
                }
            }
        }

        void CollectorEjectCubeAction::cancel() {
            auto intake = getCollector().getIntake() ;
            auto intakeaction = std::make_shared<SingleMotorPowerAction>(*intake, 0.0) ;
            intake->setAction(intakeaction) ;
            setDone();
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
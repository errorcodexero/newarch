#include "TankDriveScrubCharAction.h"
#include <tankdrive/TankDrive.h>
#include <MessageLogger.h>
#include <Robot.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {

        std::vector<std::string> TankDriveScrubCharAction::columns_ =
        {
            "time",
            "pos",
            "vel",
            "accel",
            "out"
        } ;

        TankDriveScrubCharAction::TankDriveScrubCharAction(TankDrive &drive, double power, double total, bool highgear) : TankDriveAction(drive) {
            power_ = power ;
            total_ = total ;
            high_gear_ = highgear ;

            plot_id_ = drive.initPlot("ScrubAction");
        }

        TankDriveScrubCharAction::~TankDriveScrubCharAction() {           
        }

        void TankDriveScrubCharAction::start() {
            TankDriveAction::start();

            start_time_ = getTankDrive().getRobot().getTime();
            start_angle_ = getTankDrive().getTotalAngle() ;
            start_left_ = getTankDrive().getLeftDistance() ;
            start_right_ = getTankDrive().getRightDistance() ;
            if (high_gear_)
                getTankDrive().highGear() ;
            else
                getTankDrive().lowGear() ;
                
            setMotorsToPercents(-power_, power_) ;

            auto &logger = getTankDrive().getRobot().getMessageLogger();
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
            logger << "Elapsed,angle,left,right";
            logger.endMessage();

            getTankDrive().startPlot(plot_id_, columns_);
        }

        void TankDriveScrubCharAction::run() {
            auto &logger = getTankDrive().getRobot().getMessageLogger();

            double elapsed = getTankDrive().getRobot().getTime() - start_time_;
            double angle = getTankDrive().getTotalAngle() - start_angle_ ;
            double distl = getTankDrive().getLeftDistance() - start_left_ ;
            double distr = getTankDrive().getRightDistance() - start_right_ ;

            if (angle > total_)
            {
                setDone();
                setMotorsToPercents(0.0, 0.0) ;

                double avgc = (distl - distr) / 2.0 ;
                double revs = angle / 360.0 ;
                double effr = avgc / (xero::math::PI * revs) ;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
                logger << "Total Angle (NaVX) " << angle;
                logger << ", left " << distl;
                logger << ", right " << distr;
                logger << "effective Width " << effr * 2.0;
            }
            else
            {
                logger << elapsed ;
                logger << "," << angle ;
                logger<<  "," << distl ;
                logger << "," << distr ;
                logger.endMessage();

                std::vector<double> data;
                data.push_back(elapsed);
                data.push_back(angle);
                data.push_back(getTankDrive().getAngularVelocity());
                data.push_back(getTankDrive().getAngularAcceleration());
                data.push_back(power_);
                getTankDrive().addPlotData(plot_id_, data);
            }
        }

        void TankDriveScrubCharAction::cancel() {
            setDone();
            setMotorsToPercents(0.0, 0.0);
        }

        std::string TankDriveScrubCharAction::toString() {
            std::string ret("TankDriveScrubCharAction") ;
            return ret ;
        }
    }
}

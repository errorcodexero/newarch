#include "TankDriveAngleAction.h"
#include <tankdrive/TankDrive.h>
#include <Robot.h>
#include <iostream>
#include <cmath>

using namespace xero::misc;

namespace xero
{
    namespace base
    {
        std::vector<std::string> TankDriveAngleAction::columns_ =
        {
            "time",
            "apos",
            "tpos",
            "tvel",
            "avel",
            "taccel",
            "aaccel",
            "out"
        } ;

        TankDriveAngleAction::TankDriveAngleAction(TankDrive &tank_drive, double target_angle, bool relative) : TankDriveAction(tank_drive)
        {
            SettingsParser &parser = tank_drive.getRobot().getSettingsParser();

            target_angle_ = target_angle;
            is_done_ = false;
            double maxa = parser.getDouble("tankdrive:angle_action:maxa");
            double maxd = parser.getDouble("tankdrive:angle_action:maxd");
            double maxv = parser.getDouble("tankdrive:angle_action:maxv");
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv);

            velocity_pid_ = std::make_shared<PIDACtrl>(parser, "tankdrive:angle_action:kv", 
                    "tankdrive:angle_action:kv", "tankdrive:angle_action:kp", "tankdrive:angle_action:kd", true);
            angle_threshold_ = parser.getDouble("tankdrive:angle_action:angle_threshold");

            plot_id_ = tank_drive.initPlot(toString());
        }

        TankDriveAngleAction::TankDriveAngleAction(TankDrive &tank_drive, const std::string &name, bool relative) : TankDriveAction(tank_drive)
        {
            SettingsParser &parser = tank_drive.getRobot().getSettingsParser();

            target_angle_ = tank_drive.getRobot().getSettingsParser().getDouble(name);
            is_done_ = false;
            double maxa = parser.getDouble("tankdrive:angle_action:maxa");
            double maxd = parser.getDouble("tankdrive:angle_action:maxd");
            double maxv = parser.getDouble("tankdrive:angle_action:maxv");
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv);

            velocity_pid_ = std::make_shared<PIDACtrl>(parser, "tankdrive:angle_action:kv", 
                    "tankdrive:angle_action:kv", "tankdrive:angle_action:kp", "tankdrive:angle_action:kd", true);  
            angle_threshold_ = parser.getDouble("tankdrive:angle_action:angle_threshold");

            plot_id_ = tank_drive.initPlot(toString());            
        }

        TankDriveAngleAction::~TankDriveAngleAction()
        {
        }

        void TankDriveAngleAction::start()
        {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;
            double distance = 0.0;

            if (relative_)
            {
                distance = target_angle_;
            }
            else
            {
                distance = xero::math::normalizeAngleDegrees(target_angle_ - getTankDrive().getAngle());
            }

            if (std::fabs(distance) < angle_threshold_)
            {
                is_done_ = true;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << "TankDriveAngleAction completed in start";
                logger.endMessage() ;                
            }
            else
            {
                start_time_ = getTankDrive().getRobot().getTime();
                start_angle_ = getTankDrive().getAngle();
                profile_->update(distance, 0.0, 0.0);
                
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << "Initial Profile: " << profile_->toString() ;
                logger.endMessage() ;

                getTankDrive().startPlot(plot_id_, columns_);
            }
        }

        void TankDriveAngleAction::run()
        {
            if (!is_done_)
            {
                double elapsed = getTankDrive().getRobot().getTime() - start_time_;

                if (elapsed < profile_->getTotalTime())
                {
                    double current_angle = getTankDrive().getAngle() - start_angle_;
                    double plan_angle = profile_->getDistance(elapsed);
                    double vel = profile_->getVelocity(elapsed);
                    double acc = profile_->getAccel(elapsed);

                    double out = velocity_pid_->getOutput(acc, vel, plan_angle, current_angle, getTankDrive().getRobot().getDeltaTime());
                    setMotorsToPercents(-out, out);

                    std::vector<double> data;
                    data.push_back(elapsed);
                    data.push_back(current_angle);
                    data.push_back(plan_angle);
                    data.push_back(profile_->getVelocity(elapsed));
                    data.push_back(getTankDrive().getAngularVelocity());
                    data.push_back(profile_->getAccel(elapsed));
                    data.push_back(getTankDrive().getAngularAcceleration());
                    data.push_back(out);
                    getTankDrive().addPlotData(plot_id_, data);
                }
                else
                {
                    is_done_ = true;
                    getTankDrive().endPlot(plot_id_);
                }
            }
        }

        void TankDriveAngleAction::cancel()
        {
            is_done_ = true ;
            setMotorsToPercents(0, 0) ;
        }

        bool TankDriveAngleAction::isDone()
        {
            return is_done_;
        }

        std::string TankDriveAngleAction::toString()
        {
            return "TankDriveAngleAction " + std::to_string(target_angle_);
        }
    } // namespace base
} // namespace xero

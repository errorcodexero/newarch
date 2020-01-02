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
            "out",
            "angle"
        } ;

        TankDriveAngleAction::TankDriveAngleAction(TankDrive &tank_drive, double target_angle, bool relative) : TankDriveAction(tank_drive)
        {
            SettingsParser &parser = tank_drive.getRobot().getSettingsParser();

            target_angle_ = target_angle;
            double maxa = parser.getDouble("tankdrive:angle_action:maxa");
            double maxd = parser.getDouble("tankdrive:angle_action:maxd");
            double maxv = parser.getDouble("tankdrive:angle_action:maxv");
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv);

            velocity_pid_ = std::make_shared<PIDACtrl>(parser, "tankdrive:angle_action:kv", 
                    "tankdrive:angle_action:ka", "tankdrive:angle_action:kp", "tankdrive:angle_action:kd", true);
            angle_threshold_ = parser.getDouble("tankdrive:angle_action:angle_threshold");

            plot_id_ = tank_drive.initPlot(toString());

            relative_ = relative;
        }

        TankDriveAngleAction::TankDriveAngleAction(TankDrive &tank_drive, const std::string &name, bool relative) : TankDriveAction(tank_drive)
        {
            SettingsParser &parser = tank_drive.getRobot().getSettingsParser();

            target_angle_ = tank_drive.getRobot().getSettingsParser().getDouble(name);
            double maxa = parser.getDouble("tankdrive:angle_action:maxa");
            double maxd = parser.getDouble("tankdrive:angle_action:maxd");
            double maxv = parser.getDouble("tankdrive:angle_action:maxv");
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv);

            velocity_pid_ = std::make_shared<PIDACtrl>(parser, "tankdrive:angle_action:kv", 
                    "tankdrive:angle_action:ka", "tankdrive:angle_action:kp", "tankdrive:angle_action:kd", true);  
            angle_threshold_ = parser.getDouble("tankdrive:angle_action:angle_threshold");

            plot_id_ = tank_drive.initPlot(toString());  
            relative_ = relative;                      
        }

        TankDriveAngleAction::~TankDriveAngleAction()
        {
        }

        void TankDriveAngleAction::start()
        {
            TankDriveAction::start();
            
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;
            double distance = 0.0;

            if (relative_)
            {
                distance = target_angle_;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << "TankDriveAngleAction start, relative " << distance;
                logger.endMessage() ;                  
            }
            else
            {
                distance = xero::math::normalizeAngleDegrees(target_angle_ - getTankDrive().getAngle());
                logger << "TankDriveAngleAction start, absolute " << distance;                
            }

            if (std::fabs(distance) < angle_threshold_)
            {
                setDone();
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
            TankDriveAction::run();
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
                data.push_back(getTankDrive().getAngle());
                getTankDrive().addPlotData(plot_id_, data);
            }
            else
            {
                setDone();
                getTankDrive().endPlot(plot_id_);
                setMotorsToPercents(0, 0);
            }
        }

        void TankDriveAngleAction::cancel()
        {
            TankDriveAction::cancel();
            setDone();
            setMotorsToPercents(0, 0) ;
        }

        std::string TankDriveAngleAction::toString()
        {
            return "TankDriveAngleAction " + std::to_string(target_angle_);
        }
    } // namespace base
} // namespace xero

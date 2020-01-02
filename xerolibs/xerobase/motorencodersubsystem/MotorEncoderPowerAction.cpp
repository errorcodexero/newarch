#include "MotorEncoderPowerAction.h"
#include "MotorEncoderSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> MotorEncoderPowerAction::columns_ =
        {
            "time",
            "pos",
            "vel",
            "accel",
            "out"
        } ;

        MotorEncoderPowerAction::MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, double v) : SingleMotorPowerAction(subsystem, v)
        {
            plot_id_ = -1;
        }

        MotorEncoderPowerAction::MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, const std::string &name) : SingleMotorPowerAction(subsystem, name) 
        {
            plot_id_ = -1;
        }

        MotorEncoderPowerAction::MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, double v, double d) : SingleMotorPowerAction(subsystem, v, d) 
        {
            plot_id_ = subsystem.initPlot(subsystem.getName() + "-power");
        }

        MotorEncoderPowerAction::MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, const std::string &name, 
                                                        const std::string &durname) : SingleMotorPowerAction(subsystem, name, durname) 
        {
            plot_id_ = subsystem.initPlot(subsystem.getName() + "-power");
        }           

        MotorEncoderPowerAction::~MotorEncoderPowerAction() {
        }

        void MotorEncoderPowerAction::start() {
            SingleMotorPowerAction::start();

            if (plot_id_ != -1)
            {
                getSubsystem().startPlot(plot_id_, columns_);
            }
        }

        void MotorEncoderPowerAction::run() {
            SingleMotorPowerAction::run();

            if (plot_id_ != -1)
            {
                MotorEncoderSubsystem &sub = dynamic_cast<MotorEncoderSubsystem &>(getSubsystem());

                std::vector<double> data;
                data.push_back(getElapsed());
                data.push_back(sub.getPosition());
                data.push_back(sub.getSpeedometer().getVelocity());
                data.push_back(sub.getSpeedometer().getAcceleration());
                data.push_back(getPower());

                sub.addPlotData(plot_id_, data);
            }

            if (isDone() && plot_id_ != -1)
            {
                getSubsystem().endPlot(plot_id_);
                plot_id_ = -1;
            }
        }

        void MotorEncoderPowerAction::cancel() {
            SingleMotorPowerAction::cancel();
        }
    }
}

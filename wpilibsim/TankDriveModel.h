#pragma once

#include "SubsystemModel.h"
#include <TalonSRX.h>
#include <Encoder.h>
#include <AHRS.h>
#include <vector>

namespace xero
{
    namespace sim
    {
        class RobotSimBase ;

        class TankDriveModel : public SubsystemModel
        {
          private:
            static constexpr double PI = 3.14159265359;

          public:
            TankDriveModel(RobotSimBase &);
            virtual ~TankDriveModel();

            virtual void run(double dt);
            virtual void inputChanged(SimulatedObject *obj);
            virtual std::string toString()  ;
			virtual void init() ;			

            virtual void addTalon(ctre::phoenix::motorcontrol::can::TalonSRX *motor);
            virtual void addEncoder(frc::Encoder *encoder);
            virtual void addNavX(AHRS *navx);

            double getXPos() { 
              return xpos_ ;
            }

            double getYPos() {
              return ypos_ ;
            }

            double getAngle() {
              return angle_ ;
            }

            double getSpeed() {
              return speed_ ;
            }

            double getMaxSpeed() {
              return max_speed_ ;
            }

          private:
            void updatePosition(double dx, double dy, double angle) ;

			double capValue(double prev, double desired, double maxchange) ;

          private:
			bool inited_ ;

            double left_;
            double right_;
            double angle_;
            double ticks_per_rev_;
            double diameter_;
            double left_volts_;
            double right_volts_;
            double scrub_;
            double width_;
            double right_rps_per_volt_per_time_;
            double left_rps_per_volt_per_time_;
            double time_interval_;
            double last_output_;
            double speed_ ;
            double max_speed_ ;
			double max_change_ ;
			double current_left_rps_ ;
			double current_right_rps_ ;

            double xpos_ ;
            double ypos_ ;

            double last_xpos_ ;
            double last_ypos_ ;

            int left_enc_value_ ;
            int right_enc_value_ ;

            std::vector<ctre::phoenix::motorcontrol::can::TalonSRX *> left_motors_;
            std::vector<ctre::phoenix::motorcontrol::can::TalonSRX *> right_motors_;
            frc::Encoder *left_enc_;
            frc::Encoder *right_enc_;
            AHRS *navx_;
        };
    } // namespace sim
} // namespace xero

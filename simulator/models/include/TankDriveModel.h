#pragma once

#include <frc/SubsystemModel.h>
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

            virtual void addDevice(ctre::phoenix::motorcontrol::can::TalonSRX *motor);
            virtual void addDevice(frc::Encoder *encoder);
            virtual void addDevice(AHRS *navx);
            virtual void addDevice(frc::Solenoid *sol) ;

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

            void generateDisplayInformation(std::list<std::string> &lines) ;

        private:
            void updatePosition(double dx, double dy, double angle) ;

            double capValue(double prev, double desired, double maxchange) ;

            void lowGear() ;
            void highGear() ;

            void calcLowLevelParams(RobotSimBase &simbase) ;

          private:
            bool inited_ ;
            bool gear_ ;

            double left_;
            double right_;
            double angle_;
            double navx_offset_ ;
            double ticks_per_rev_;
            double diameter_;
            double left_volts_;
            double right_volts_;
            double scrub_;
            double width_;

            double high_rps_per_volt_per_time_;
            double low_rps_per_volt_per_time_;
            double high_max_change_ ;
            double low_max_change_ ;
            double current_max_change_ ;

            double left_right_error_ ;
            double right_rps_per_volt_per_time_;
            double left_rps_per_volt_per_time_;                     
            double time_interval_;
            double last_output_;
            double speed_ ;

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
            frc::Solenoid *shifter_ ;
            AHRS *navx_;
        };
    } // namespace sim
} // namespace xero

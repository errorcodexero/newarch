#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <memory>

namespace xero
{
    namespace models
    {
        class WestCoastDrive : public xero::sim2::SimulationModel
        {
        public:
            WestCoastDrive(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~WestCoastDrive();

            virtual bool create() ;
            virtual void run(double dt);

        private:
            double capValue(double prev, double target, double maxchange);
            void updatePosition(double dl, double dr, double angle);

            void calcLowLevelParams();
            void calcHighLevelParams();
            void calcGearDependentParameters();

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> left_motor_ ;
            std::shared_ptr<xero::sim2::SimulatedMotor> right_motor_ ;
            int left_enc_[2] ;
            int right_enc_[2] ;

            double diameter_;
            double width_;

            int gear_;

            double left_rps_per_power_per_time_;
            double right_rps_per_power_per_time_;
            double left_motor_mult_;
            double right_motor_mult_;

            double current_left_rps_;
            double current_right_rps_;

            double current_max_change_;

            double left_;
            double right_;

            double scrub_;

            double angle_;
            double last_angle_;
            double total_angle_;

            double xpos_;
            double ypos_;

            double last_xpos_;
            double last_ypos_;

            double speed_;

            double ticks_per_rev_;
            double left_encoder_mult_;
            double right_encoder_mult_;

            int32_t left_enc_value_ ;
            int32_t right_enc_value_;
        };
    }
}
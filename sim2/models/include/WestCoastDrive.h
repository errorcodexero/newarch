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
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            enum class Gear
            {
                HighGear,
                LowGear
            } ;

        private:
            double capValue(double prev, double target, double maxchange);
            void updatePosition(double dl, double dr, double angle);

            bool calcLowLevelParams();

            void setGear() ;
            void highGear() ;
            void lowGear() ;

            bool getDriveParameter(const std::string &name, double &value) ; 

        private:
            constexpr static const char *UseEncodersName = "hw:use_motor_encoders" ;
            constexpr static const char *LeftMotorName = "hw:left:motor" ;
            constexpr static const char *RightMotorName = "hw:right:motor" ;
            constexpr static const char *LeftEncoderOneName = "hw:left:encoder:1" ;
            constexpr static const char *LeftEncoderTwoName = "hw:left:encoder:2" ;
            constexpr static const char *RightEncoderOneName = "hw:right:encoder:1" ;
            constexpr static const char *RightEncoderTwoName = "hw:right:encoder:2" ;
            constexpr static const char *ShifterName = "shifter" ;
            constexpr static const char *ShifterInvertedName = "shifterinverted" ;
            constexpr static const char *InvertLeftMotor = "invertleft" ;
            constexpr static const char *InvertRightMotor = "invertright" ;

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> left_motor_ ;
            std::shared_ptr<xero::sim2::SimulatedMotor> right_motor_ ;
            int left_enc_[2] ;
            int right_enc_[2] ;
            bool use_motor_encoders_ ;

            int shifter_ ;
            bool shifter_inverted_ ;

            double diameter_;
            double width_;
            double length_ ;

            Gear gear_;

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

            double high_rps_per_power_per_time_ ;
            double low_rps_per_power_per_time_ ;            
            double high_max_change_ ;
            double low_max_change_ ;

            std::vector<double> data_ ;

            static std::vector<std::string> ValueNames ;
        };
    }
}
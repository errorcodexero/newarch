#include <WestCoastDrive.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <xeromath.h>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        WestCoastDrive::WestCoastDrive(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "westcoastdrive", inst)
        {
        }

        bool WestCoastDrive::create()
        {
            try
            {
                left_motor_ = std::make_shared<SimulatedMotor>(*this, "hw:left:motor") ;
                right_motor_ = std::make_shared<SimulatedMotor>(*this, "hw:right:motor") ;                
            }
            catch(const std::runtime_error &e)
            {
                return false;
            }

            left_enc_[0] = getInteger("hw:left:encoder:1") ;
            left_enc_[0] = getInteger("hw:left:encoder:2") ;
            right_enc_[0] = getInteger("hw:right:encoder:1") ;
            right_enc_[1] = getInteger("hw:right:encoder:2") ;

            return true ;
        }

        WestCoastDrive::~WestCoastDrive()
        {
        }

        void WestCoastDrive::updatePosition(double dl, double dr, double angle) 
        {
            if (std::fabs(dl - dr) < 1e-6) {
                // Straight line
                xpos_ += dl * std::cos(angle) ;
                ypos_ += dr * std::sin(angle) ;
            }
            else {
                double r = width_ * (dl + dr) / (2 * (dr - dl)) ;
                double wd = (dr - dl) / width_ ;
                xpos_ = xpos_ + r * std::sin(wd + angle) - r * std::sin(angle) ;
                ypos_ = ypos_ - r * std::cos(wd + angle) + r * std::cos(angle) ;
            }

            double dangle = xero::math::normalizeAngleRadians(angle_ - last_angle_);
            total_angle_ += dangle;
        }        

        void WestCoastDrive::run(double dt) {
            double leftpower = left_motor_->Get();
            double rightpower = right_motor_->Get();

            //
            // Calculate the new desired revolutions per second (RPS)
            //
            double desired_left_rps = leftpower * left_rps_per_power_per_time_  * left_motor_mult_ ;
            double desired_right_rps = rightpower * right_rps_per_power_per_time_  * right_motor_mult_ ;

            //
            // Now, see how much our RPS can actually changes
            //
            current_left_rps_ = capValue(current_left_rps_, desired_left_rps, current_max_change_) ;
            current_right_rps_ = capValue(current_right_rps_, desired_right_rps, current_max_change_) ;

            //
            // Now, calculate distance each wheel moved based on the actual RPS
            //
            double dleft = current_left_rps_* dt * diameter_ * PI;
            double dright = current_right_rps_ * dt * diameter_ * PI;

            //
            // And add to the total distance so far
            //
            left_ += dleft;
            right_ += dright;

            double lrevs = left_ / (PI * diameter_);
            double rrevs = right_ / (PI * diameter_) ;

            double dv = (dright - dleft) / 2 * scrub_;
            angle_ = xero::math::normalizeAngleRadians(angle_ + (dv * 2.0) / width_) ;
            updatePosition(dleft, dright, angle_) ;

            double dist = std::sqrt((xpos_ - last_xpos_) * (xpos_ - last_xpos_) + (ypos_ - last_ypos_) * (ypos_ - last_ypos_)) ;
            speed_ = dist / dt ;

            last_xpos_ = xpos_ ;
            last_ypos_ = ypos_ ;

            left_enc_value_ = static_cast<int32_t>(lrevs * ticks_per_rev_ * left_encoder_mult_) ;
            right_enc_value_ = static_cast<int32_t>(rrevs * ticks_per_rev_ * right_encoder_mult_) ;

#ifdef NOTYET
            if (left_enc_ != nullptr)
                left_enc_->SimulatorSetValue(left_enc_value_) ;

            if (right_enc_ != nullptr)
                right_enc_->SimulatorSetValue(right_enc_value_) ;

            if (left_spark_encoder_ != nullptr)
                left_spark_encoder_->SimulatorSetPosition(left_enc_value_);

            if (right_spark_encoder_ != nullptr)
                right_spark_encoder_->SimulatorSetPosition(right_enc_value_);

            if (navx_ != nullptr) {
                double deg = normalizeAngleDegrees(-rad2deg(angle_ + navx_offset_)) ;
                navx_->SimulatorSetYaw(deg) ;
                navx_->SimulatorSetTotalAngle(-rad2deg(total_angle_ + navx_offset_));
            }
#endif

        }

        
        double WestCoastDrive::capValue(double prev, double target, double maxchange) {
            double ret ;

            if (target > prev) {
                if (target > prev + maxchange)
                    ret = prev + maxchange ;
                else
                    ret = target ;
            }
            else {
                if (target < prev - maxchange)
                    ret = prev - maxchange ;
                else
                    ret = target ;
            }

            return ret ;
        }

        void WestCoastDrive::calcLowLevelParams() {
#ifdef NOTYET            
            double maxhighvel = simbase.getSettingsParser().getDouble("tankdrive:high:maxvelocity") ;
            double maxhighaccel = simbase.getSettingsParser().getDouble("tankdrive:high:maxacceleration") ;
            double maxlowvel = simbase.getSettingsParser().getDouble("tankdrive:low:maxvelocity") ;
            double maxlowaccel = simbase.getSettingsParser().getDouble("tankdrive:low:maxacceleration") ;

            double circum = diameter_ * PI ;

            high_rps_per_power_per_time_ = maxhighvel / circum ;
            low_rps_per_power_per_time_ = maxlowvel / circum ;
            high_max_change_ = maxhighaccel / circum ;
            low_max_change_ = maxlowaccel / circum ;
#endif            
        }

        void WestCoastDrive::calcGearDependentParameters() {
#ifdef NOTYET            
            if (left_right_error_ > 1e-6) {
                std::uniform_real_distribution<double> unif(0.95, 1.05) ;
                std::default_random_engine re ;
                right_rps_per_power_per_time_ = low_rps_per_power_per_time_ * unif(re) ;
                left_rps_per_power_per_time_ = low_rps_per_power_per_time_ * unif(re) ;
                current_max_change_ = low_max_change_ ;                
            }
            else {
                right_rps_per_power_per_time_ = low_rps_per_power_per_time_  ;
                left_rps_per_power_per_time_ = low_rps_per_power_per_time_ ;
                current_max_change_ = low_max_change_ ;
            }

            gear_ = Gear::LowGear ;
#endif
        }
    }
}

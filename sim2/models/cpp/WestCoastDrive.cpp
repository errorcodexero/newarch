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
            use_motor_encoders_ = false ;
            try
            {
                left_motor_ = std::make_shared<SimulatedMotor>(*this, LeftMotorName) ;
                right_motor_ = std::make_shared<SimulatedMotor>(*this, RightMotorName) ;                
            }
            catch(const std::runtime_error &e)
            {
                return false;
            }

            dumpProperties() ;

            if (hasProperty(UseEncodersName))
            {
                const SimValue &v = getProperty(UseEncodersName) ;
                if (!v.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Error) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'use_motor_encoders' but type is not boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return false ;
                }

                if (v.getBoolean())
                    use_motor_encoders_ = true ;
            }

            if (!use_motor_encoders_)
            {
                left_enc_[0] = getInteger(LeftEncoderOneName) ;
                left_enc_[1] = getInteger(LeftEncoderTwoName) ;
                right_enc_[0] = getInteger(RightEncoderOneName) ;
                right_enc_[1] = getInteger(RightEncoderTwoName) ;
            }

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

        void WestCoastDrive::run(uint64_t microdt) {
            double timesec = static_cast<double>(microdt) / 1.0e6 ;
            double leftpower = left_motor_->get();
            double rightpower = right_motor_->get();

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
            double dleft = current_left_rps_* timesec * diameter_ * xero::math::PI  ;
            double dright = current_right_rps_ * timesec * diameter_ * xero::math::PI ;

            //
            // And add to the total distance so far
            //
            left_ += dleft;
            right_ += dright;

            double lrevs = left_ / (xero::math::PI * diameter_);
            double rrevs = right_ / (xero::math::PI * diameter_);

            double dv = (dright - dleft) / 2 * scrub_;
            angle_ = xero::math::normalizeAngleRadians(angle_ + (dv * 2.0) / width_) ;
            updatePosition(dleft, dright, angle_) ;

            double dist = std::sqrt((xpos_ - last_xpos_) * (xpos_ - last_xpos_) + (ypos_ - last_ypos_) * (ypos_ - last_ypos_)) ;
            speed_ = dist / timesec ;

            last_xpos_ = xpos_ ;
            last_ypos_ = ypos_ ;

            left_enc_value_ = static_cast<int32_t>(lrevs * ticks_per_rev_ * left_encoder_mult_) ;
            right_enc_value_ = static_cast<int32_t>(rrevs * ticks_per_rev_ * right_encoder_mult_) ;

            if (use_motor_encoders_)
            {
                left_motor_->setEncoder(left_enc_value_) ;
                right_motor_->setEncoder(right_enc_value_) ;
            }
            else
            {
                getEngine().setEncoder(left_enc_[0], left_enc_[1], left_enc_value_) ;
                getEngine().setEncoder(right_enc_[0], right_enc_[1], right_enc_value_) ;                
            }

#ifdef NOTYET
            if (navx_ != nullptr) {
                double deg = xero::math::normalizeAngleDegrees(-rad2deg(angle_ + navx_offset_)) ;
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

#include <WestCoastDrive.h>
#include <NavXSim.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <xeromath.h>
#include <mockdata/PCMData.h>
#include <iostream>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        std::vector<std::string> WestCoastDrive::ValueNames = 
        {
            "X",
            "Y",
            "ANGLE"
        } ;

        WestCoastDrive::WestCoastDrive(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "westcoastdrive", inst)
        {
            shifter_ = -1 ;
            shifter_inverted_ = false ;
        }

        WestCoastDrive::~WestCoastDrive()
        {
        }        

        void WestCoastDrive::processEvent(const std::string &name, const SimValue &value)
        {
            if (name == "xpos")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' is valid, but type must be double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return ;
                }

                xpos_ = value.getDouble() ;
            }
            else if (name == "ypos")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' is valid, but type must be double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return ;
                }

                ypos_ = value.getDouble() ;
            }     
            else if (name == "angle")
            {
                if (!value.isDouble())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Warning) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - event '" << name << "' is valid, but type must be double" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return ;
                }

                angle_ = xero::math::deg2rad(value.getDouble()) ;
            }                     
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

            if (!calcLowLevelParams())
                return false ;

            left_motor_mult_ = 1.0 ;
            right_motor_mult_ = 1.0 ;

            if (hasProperty(InvertLeftMotor))
            {
                const SimValue &v = getProperty(InvertLeftMotor) ;
                if (!v.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Error) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property '" << InvertLeftMotor << "' but type is not boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return false ;
                }

                if (v.getBoolean())
                    left_motor_mult_ = -1 ;
            }

            if (hasProperty(InvertRightMotor))
            {
                const SimValue &v = getProperty(InvertRightMotor) ;
                if (!v.isBoolean())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Error) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property '" << InvertRightMotor << "' but type is not boolean" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return false ;
                }

                if (v.getBoolean())
                    right_motor_mult_ = -1 ;
            }           

            left_encoder_mult_ = 1.0 ;
            right_encoder_mult_ = 1.0 ;

            setGear() ;

            if (hasProperty(ShifterName))
            {
                const SimValue &v = getProperty(ShifterName) ;
                if (!v.isInteger())
                {
                    SimulatorMessages &msg = getEngine().getMessageOutput() ;
                    msg.startMessage(SimulatorMessages::MessageType::Error) ;
                    msg << "model " << getModelName() << " instance " << getInstanceName() ;
                    msg << " - has property 'shifter' but type is not an integer" ;
                    msg.endMessage(getEngine().getSimulationTime()) ;
                    return false ;
                }

                shifter_ = v.getInteger() ;
            }

            registerDataFormat("drivebase", ValueNames) ;
            return true ;
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

        void WestCoastDrive::setGear()
        {
            if (shifter_ == -1) 
            {
                highGear() ;
            }
            else
            {
                if (HALSIM_GetPCMSolenoidOutput(0, shifter_))
                {
                    //
                    // Shifter is enabled
                    //
                    if (shifter_inverted_)
                        lowGear() ;
                    else
                        highGear() ;
                }
                else
                {
                    //
                    // Shifter is NOT enabled
                    //
                    if (shifter_inverted_)
                        highGear() ;
                    else
                        lowGear() ;
                }
            }
        }

        void WestCoastDrive::run(uint64_t microdt) 
        {
            double timesec = static_cast<double>(microdt) / 1.0e6 ;

            setGear() ;

            double leftpower = left_motor_->get();
            double rightpower = right_motor_->get();

            if (std::fabs(leftpower) > 0.01 || std::fabs(rightpower) > 0.01)
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Debug, 10) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - is being actively powered" ;
                msg.endMessage(getEngine().getSimulationTime()) ;                
            }

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

            double deg = xero::math::normalizeAngleDegrees(-xero::math::rad2deg(angle_)) ;
            NavXSim *navx = NavXSim::getNavXSim() ;
            if (navx != nullptr)
                navx->setYaw(deg) ;

            data_.clear() ;
            data_.push_back(xpos_) ;
            data_.push_back(ypos_) ;
            data_.push_back(deg) ;
            presentDataValues(data_) ;
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

        bool WestCoastDrive::getDriveParameter(const std::string &name, double &value)
        {
            if (!hasProperty(name))
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - missing required property '" << name << "'" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;                
            }

            const SimValue &prop = getProperty(name) ;
            if (!prop.isDouble())
            {
                SimulatorMessages &msg = getEngine().getMessageOutput() ;
                msg.startMessage(SimulatorMessages::MessageType::Error) ;
                msg << "model " << getModelName() << " instance " << getInstanceName() ;
                msg << " - has required property '" << name << "' but it is not a double" ;
                msg.endMessage(getEngine().getSimulationTime()) ;
                return false ;                 
            }

            value = prop.getDouble() ;
            return true ;
        }

        bool WestCoastDrive::calcLowLevelParams() 
        {
            double maxhighvel ;
            double maxhighaccel ;
            double maxlowvel ;
            double maxlowaccel ;
            double d;

            if (!getDriveParameter("diameter", d))
                return false ;
            diameter_ = d ;

            if (!getDriveParameter("scrub", d))
                return false ;
            scrub_ = d ;

            if (!getDriveParameter("width", d))
                return false ;
            width_ = d ;

            if (!getDriveParameter("length", d))
                return false ;
            length_ = d ;

            if (!getDriveParameter("ticks_per_rev", d))
                return false ;
            ticks_per_rev_ = d ;

            if (!getDriveParameter("high:maxvelocity", maxhighvel))
                return false ;

            if (!getDriveParameter("high:maxaccel", maxhighaccel))
                return false ;

            if (!getDriveParameter("low:maxvelocity", maxlowvel))
                return false ;

            if (!getDriveParameter("low:maxaccel", maxlowaccel))
                return false ;                                                

            double circum = diameter_ * xero::math::PI ;

            high_rps_per_power_per_time_ = maxhighvel / circum ;
            low_rps_per_power_per_time_ = maxlowvel / circum ;
            high_max_change_ = maxhighaccel / circum ;
            low_max_change_ = maxlowaccel / circum ;

            return true ;
        }

        void WestCoastDrive::lowGear() {
            right_rps_per_power_per_time_ = low_rps_per_power_per_time_  ;
            left_rps_per_power_per_time_ = low_rps_per_power_per_time_ ;
            current_max_change_ = low_max_change_ ;
            gear_ = Gear::LowGear ;
        }

        void WestCoastDrive::highGear() {
            right_rps_per_power_per_time_ = high_rps_per_power_per_time_  ;
            left_rps_per_power_per_time_ = high_rps_per_power_per_time_ ;
            current_max_change_ = high_max_change_ ;
            gear_ = Gear::HighGear ;
        }        
    }
}

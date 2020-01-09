#include "XeroEncoder.h"
#include <xeromath.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc;

namespace xero {
    namespace base {
        static void invalidEncoder(MessageLogger &logger, const std::string &configName, const std::string &errorMessage) {
            logger.startMessage(MessageLogger::MessageType::error);
            logger << "Invalid encoder declaration '" << configName << "': " << errorMessage;
            logger.endMessage();
            assert(0);
        }

        XeroEncoder::XeroEncoder(Robot &robot, const std::string &configName, 
                                 bool angular, std::shared_ptr<MotorController> motor): robot_(robot) {
            auto &logger = robot.getMessageLogger();
            auto &parser = robot.getSettingsParser();

            angular_ = angular ;
            name_ = configName ;
            std::string quadName = configName + ":quad";
            if (parser.isDefined(quadName + ":motor") && parser.getBoolean(quadName + ":motor")) {
                // use an embedded quadrature encoder
                assert(motor->hasPosition());
                motor_ = motor;
            }
            else if (parser.isDefined(quadName + ":1") || parser.isDefined(quadName + ":2")) {
                // parse a quadrature encoder
                if (!parser.isDefined(quadName + ":1") || !parser.isDefined(quadName + ":2")) {
                    invalidEncoder(logger, quadName, "quadrature encoder requires both pins");
                }
                quad_ = std::make_shared<frc::Encoder>(
                    parser.getInteger(quadName + ":1"),
                    parser.getInteger(quadName + ":2")
                );
            } else quad_ = nullptr;

            if (quad_  != nullptr || motor_ != nullptr) {
                if (parser.isDefined(quadName + ":m")) quadM_ = parser.getDouble(quadName + ":m");
                else invalidEncoder(logger, quadName, "m parameter is required");

                if (parser.isDefined(quadName + ":b")) quadB_ = parser.getDouble(quadName + ":b");
                else invalidEncoder(logger, quadName, "b parameter is required");
            }

            std::string analogName = configName + ":analog";
            if (parser.isDefined(analogName)) {
                // parse an analog encoder
                analog_ = std::make_shared<frc::AnalogInput>(parser.getInteger(analogName));

                double rmin, rmax ;
                double emin, emax ;
                double rc, ec ;

                rmin = robot.getSettingsParser().getDouble(configName + ":analog:rmin") ;
                rmax = robot.getSettingsParser().getDouble(configName + ":analog:rmax") ;
                emin = robot.getSettingsParser().getDouble(configName + ":analog:emin") ;
                emax = robot.getSettingsParser().getDouble(configName + ":analog:emax") ;
                rc = robot.getSettingsParser().getDouble(configName + ":analog:rc") ;
                ec = robot.getSettingsParser().getDouble(configName + ":analog:ec") ;

                mapper_ = std::make_shared<EncoderMapper>(rmax, rmin, emax, emin) ;
                mapper_->calibrate(rc, ec) ;
            } else 
                analog_ = nullptr;

            std::string pwmName = configName + ":pwm";
            if (parser.isDefined(pwmName)) {
                // parse an analog encoder
                pwm_ = std::make_shared<frc::Counter>(parser.getInteger(pwmName));
                pwm_->SetSemiPeriodMode(true);

                double rmin, rmax ;
                double emin, emax ;
                double rc, ec ;

                rmin = robot.getSettingsParser().getDouble(configName + ":analog:rmin") ;
                rmax = robot.getSettingsParser().getDouble(configName + ":analog:rmax") ;
                emin = robot.getSettingsParser().getDouble(configName + ":analog:emin") ;
                rmax = robot.getSettingsParser().getDouble(configName + ":analog:emax") ;
                rc = robot.getSettingsParser().getDouble(configName + ":analog:rc") ;
                ec = robot.getSettingsParser().getDouble(configName + ":analog:ec") ;

                mapper_ = std::make_shared<EncoderMapper>(rmax, rmin, emax, emin) ;
                mapper_->calibrate(rc, ec) ;                

            } else 
                pwm_ = nullptr;

            // validate configuration
            if (pwm_ && analog_) invalidEncoder(logger, configName, "cannot declare two absolute encoders");
            if (!(quad_ != nullptr || pwm_ != nullptr || analog_ != nullptr || motor_ != nullptr)) {
                invalidEncoder(logger, configName, "must declare :quad:(1,2), :analog, or :pwm");
            }
        }

        double XeroEncoder::getPosition() {
            double result = 0.0 ;

            if (motor_ || quad_) 
            {
                result = quadM_ * (motor_ ? motor_->getPosition() : quad_->Get()) + quadB_;
                if (angular_) 
                    result = xero::math::normalizeAngleDegrees(result);
            } 
            else  
            {
                result = getAbsolutePosition();
            }

            return result ;
        }

        double XeroEncoder::getAbsolutePosition() {
            double pos;
            if (analog_ != nullptr)
                pos = analog_->GetVoltage();
            else if (pwm_ != nullptr)
                pos = pwm_->GetPeriod();
            else 
                assert(0 == "no absolute encoder found");

            // If the robot is disabled, output the raw hardware value for characterization
            if (robot_.IsDisabled()) 
                frc::SmartDashboard::PutNumber(name_, pos) ;

            pos = mapper_->toRobot(pos) ;            
            if (angular_)
                pos = xero::math::normalizeAngleDegrees(pos) ;

            return pos ;
        }

        void XeroEncoder::reset() {
            if (motor_)
                motor_->resetEncoder();
            else if (quad_)
                quad_->Reset();
            calibrate();
        }

        void XeroEncoder::calibrate() {
            if ((quad_ || motor_) && (analog_ || pwm_)) {
                calibrate(getAbsolutePosition()) ;
            }
        }

        void XeroEncoder::calibrate(double pos) {
            if (motor_)
                motor_->resetEncoder();
            else if (quad_)
                quad_->Reset();
            quadB_ = pos;
        }
    }
}
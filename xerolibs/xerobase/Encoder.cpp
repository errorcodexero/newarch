#include "Encoder.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        static void invalidEncoder(MessageLogger &logger, const std::string &configName, const std::string &errorMessage) {
            logger.startMessage(MessageLogger::MessageType::error);
            logger << "Invalid encoder declaration '" << configName << "': " << errorMessage;
            logger.endMessage();
            assert(0);
        }

        Encoder::Encoder(MessageLogger &logger, SettingsParser &parser, const std::string &configName) {
            std::string quadName = configName + ":quad";
            std::shared_ptr<frc::Encoder> quad;
            if (parser.isDefined(quadName + ":1") || parser.isDefined(quadName + ":2")) {
                // parse a quadrature encoder
                if (!parser.isDefined(quadName + ":1") || !parser.isDefined(quadName + ":2")) {
                    invalidEncoder(logger, quadName, "quadrature encoder requires both pins");
                }
                quad = std::make_shared<frc::Encoder>(
                    parser.getInteger(quadName + ":1"),
                    parser.getInteger(quadName + ":2")
                );
                
                if (parser.isDefined(quadName + ":m")) quadM_ = parser.getDouble(quadName + ":m");
                if (parser.isDefined(quadName + ":b")) quadB_ = parser.getDouble(quadName + ":b");
            } else quad = nullptr;

            std::string analogName = configName + ":analog";
            if (parser.isDefined(analogName)) {
                // parse an analog encoder
                analog_ = std::make_shared<frc::AnalogInput>(parser.getInteger(analogName));

                if (parser.isDefined(analogName + ":m")) absM_ = parser.getDouble(analogName + ":m");
                if (parser.isDefined(analogName + ":b")) absB_ = parser.getDouble(analogName + ":b");
            } else analog_ = nullptr;

            std::string pwmName = configName + ":pwm";
            if (parser.isDefined(pwmName)) {
                // parse an analog encoder
                pwm_ = std::make_shared<frc::Counter>(parser.getInteger(pwmName));
                pwm_->SetSemiPeriodMode(true);

                if (parser.isDefined(pwmName + ":m")) absM_ = parser.getDouble(pwmName + ":m");
                if (parser.isDefined(pwmName + ":b")) absB_ = parser.getDouble(pwmName + ":b");
            } else pwm_ = nullptr;

            // validate configuration
            if (pwm_ && analog_) invalidEncoder(logger, configName, "cannot declare two absolute encoders");
            if (!(quad_ || pwm_ || analog_)) {
                invalidEncoder(logger, configName, "must declare :quad:(1,2), :analog, or :pwm");
            }
        }

        double Encoder::getPosition() {
            if (quad_) return quadM_*quad_->Get() + quadB_;
            else return getAbsolutePosition();
        }

        double Encoder::getAbsolutePosition() {
            double pos;
            if (analog_) pos = analog_->GetVoltage();
            else if (pwm_) pos = pwm_->GetPeriod();
            else assert(0 == "no absolute encoder found");

            return absM_*pos + absB_;
        }

        void Encoder::reset() {
            if (quad_) {
                quad_->Reset();
                calibrate();
            }
        }

        void Encoder::calibrate() {
            if (quad_ && (analog_ || pwm_)) {
                quadB_ += getAbsolutePosition() - getPosition();
            }
        }
    }
}
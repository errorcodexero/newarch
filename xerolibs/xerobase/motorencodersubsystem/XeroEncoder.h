#pragma once
#include <string>
#include <memory>

#include <SettingsParser.h>
#include <MessageLogger.h>
#include <frc/Encoder.h>
#include <frc/AnalogInput.h>
#include <frc/Counter.h>

namespace xero {
    namespace base {
        class XeroEncoder {
        public:
            /// Creates an XeroEncoder from the configuration file.
            /// @param logger The message logger.
            /// @param settings The settings parser.
            /// @param configName The name of the encoder in the configuration file.
            /// XeroEncoder config syntax:
            /// To declare a quadrature encoder:
            /// configName:quad:1        1    # first pin
            /// configName:quad:2        2    # second pin
            /// configName:quad:m        100  # M constant
            /// configName:quad:b        10   # B constant
            ///
            /// To declare an analog encoder:
            /// configName:analog        1     # pin
            /// configName:analog:m      100   # M constant
            /// configName:analog:b      10    # B constant
            ///
            /// To declare a PWM encoder:
            /// configName:pwm           1     # pin
            /// configName:pwm:m         100   # M constant
            /// configName:pwm:b         10    # B constant
            ///
            /// A quadrature encoder may optionally be calibrated
            /// by an analog encoder XOR a PWM encoder.
            XeroEncoder(xero::misc::MessageLogger &logger,
                    xero::misc::SettingsParser &settings, 
                    const std::string &configName
            );

            /// Creates a quadrature encoder.
            /// @param logger The message logger.
            XeroEncoder(std::shared_ptr<frc::Encoder> quadratureEncoder
            ): quad_(quadratureEncoder) {}

            /// Creates an analog encoder, or a quadrature encoder calibrated by an analog encoder.
            /// @param analogEncoder The analog input to which the encoder is connected.
            /// @param quadratureEncoder The quadrature encoder object, or \c nullptr to use just an analog encoder.
            XeroEncoder(std::shared_ptr<frc::AnalogInput> analogEncoder,
                    std::shared_ptr<frc::Encoder>     quadratureEncoder = nullptr
            ): quad_(quadratureEncoder), analog_(analogEncoder) {}

            /// Creates a PWM encoder, or a quadrature encoder calibrated by a PWM encoder.
            /// @param quadratureEncoder The quadrature encoder object, or \c nullptr to use just a PWM encoder.
            /// @param pwmEncoder A \c Counter measuring the signal from the PWM encoder.
            XeroEncoder(std::shared_ptr<frc::Counter> pwmEncoder,
                    std::shared_ptr<frc::Encoder> quadratureEncoder = nullptr
            ): quad_(quadratureEncoder), pwm_(pwmEncoder) { pwm_->SetSemiPeriodMode(true); }

            /// Returns the current position of this encoder.
            /// If this encoder uses both a quadrature and absolute encoder,
            /// the value measured by the quadrature encoder is returned.
            double getPosition();

            /// If this encoder has an absolute encoder, returns the measured position.
            /// Asserts otherwise.
            double getAbsolutePosition();

            /// Resets the quadrature encoder to the zero position. If an absolute encoder
            /// is declared, the quadrature encoder is also calibrated.
            ///
            /// If no quadrature encoder is in use, this method has no effect.
            void reset();

            /// If this encoder uses both a quadrature and absolute encoder,
            /// this resets the B value of the quadrature encoder
            /// such that its current value matches the absolute encoder.
            ///
            /// If this encoder does not use both a quad & absolute encoder,
            /// this method has no effect.
            void calibrate();

            /// Sets the linear function mapping for the quadrature encoder.
            /// Asserts if there is no quadrature encoder.
            /// @param m The M constant.
            /// @param b The B constant.
            void setQuadMapping(double m, double b) {
                setQuadM(m);
                setQuadB(b);
            }

            /// Sets the linear function mapping for the absolute encoder.
            /// Asserts if there is no absolute encoder.
            /// @param m The M constant.
            /// @param b The B constant.
            void setAbsoluteMapping(double m, double b) {
                setAbsoluteM(m);
                setAbsoluteB(b);
            }

            /// Sets the quadrature encoder's M constant.
            /// Asserts if there is no quadrature encoder.
            /// @param m The new M constant.
            void setQuadM(double m) {
                assert(quad_);
                quadM_ = m;
            }

            /// Sets the quadrature encoder's B constant.
            /// Asserts if there is no quadrature encoder.
            /// @param b The new B constant.
            void setQuadB(double b) {
                assert(quad_);
                quadB_ = b;
            }

            /// Sets the absolute encoder's M constant.
            /// Asserts if there is no absolute encoder.
            /// @param m The new M constant.
            void setAbsoluteM(double m) {
                assert(analog_ || pwm_);
                absM_ = m;
            }

            /// Sets the absolute encoder's B constant.
            /// Asserts if there is no absolute encoder.
            /// @param b The new B constant.
            void setAbsoluteB(double b) {
                assert(analog_ || pwm_);
                absB_ = b;
            }
        private:
            std::shared_ptr<frc::Encoder> quad_;    // A quadrature encoder, or nullptr.
            double quadM_ = 1;
            double quadB_ = 0;

            std::shared_ptr<frc::AnalogInput> analog_;    // An analog encoder, or nullptr.
            std::shared_ptr<frc::Counter> pwm_;    // A PWM encoder, or nullptr.
            double absM_ = 1;
            double absB_ = 0;
        };
    }
}
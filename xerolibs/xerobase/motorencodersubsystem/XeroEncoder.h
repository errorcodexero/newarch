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
        /// @brief The XeroEncoder class is an encoder abstraction.  It is used
        /// primarily with the MoterEncoderSubsystem.
        /// A complex encoder may be a single absolute analog encoder, a single 
        /// absolute pwm encoders, or a single relative quadrature encoder.  It 
        /// also may be a relative quadrature encoder paired with either type of
        /// absolute encoder to support in place calibration.
        /// @sa MotorEncoderSubsystem
        class XeroEncoder {
        public:
            /// @brief Creates an XeroEncoder from the configuration file.
            /// 
            /// XeroEncoder config syntax: \n 
            /// To declare a quadrature encoder:
            /// @code
            ///     configName:quad:1        1    # first pin
            ///     configName:quad:2        2    # second pin
            ///     configName:quad:m        100  # M constant
            ///     configName:quad:b        10   # B constant
            /// @endcode
            /// To declare an analog encoder:
            /// @code
            ///     configName:analog        1     # pin
            ///     configName:analog:m      100   # M constant
            ///     configName:analog:b      10    # B constant
            /// @endcode
            /// To declare a PWM encoder:
            /// @code
            ///     configName:pwm           1     # pin\n 
            ///     configName:pwm:m         100   # M constant\n 
            ///     configName:pwm:b         10    # B constant\n 
            /// @endcode
            ///
            /// A quadrature encoder may optionally be calibrated by an analog encoder XOR a PWM encoder.
            /// For each type of encoder, the output position is calculated by reading the value 
            /// from the encoder and applying a linear function (Y = MX + B) to the value.  The M and B
            /// values must be contained in the settings file, but can be overridden with methods found on
            /// the class.\n 
            /// This class supports calibration is the act of ensuring that the position returned from the
            /// encoder matches the position of the mechanism being monitored on the robot.  Calibration 
            /// @param logger The message logger.
            /// @param settings The settings parser.
            /// @param configName The name of the encoder in the configuration file.
            /// @param angular true if the encoder is angular
            XeroEncoder(xero::misc::MessageLogger &logger,
                    xero::misc::SettingsParser &settings, 
                    const std::string &configName,
                    bool angular = false
            );

            /// @brief Creates a quadrature encoder.
            /// If angular is true, it is assumed the position is in degrees and the
            /// values are normalized to be between -180 and +180 degrees.
            /// @param quadratureEncoder an FRC quadrature encoder object
            /// @param angular true if this encoder measures an angle
            XeroEncoder(std::shared_ptr<frc::Encoder> quadratureEncoder, bool angular = false
            ): angular_(angular), quad_(quadratureEncoder) {}

            /// Creates an analog encoder, or a quadrature encoder calibrated by an analog encoder.
            /// @param analogEncoder The analog input to which the encoder is connected.
            /// @param quadratureEncoder The quadrature encoder object, or \c nullptr to use just an analog encoder.
            /// @param angular true if this encoder measures an angle
            XeroEncoder(std::shared_ptr<frc::AnalogInput> analogEncoder,
                        std::shared_ptr<frc::Encoder>     quadratureEncoder = nullptr,
                        bool angular = false
            ): angular_(angular), quad_(quadratureEncoder), analog_(analogEncoder) {}

            /// Creates a PWM encoder, or a quadrature encoder calibrated by a PWM encoder.
            /// @param quadratureEncoder The quadrature encoder object, or \c nullptr to use just a PWM encoder.
            /// @param pwmEncoder A \c Counter measuring the signal from the PWM encoder.
            /// @param angular true if this encoder measures an angle
            XeroEncoder(std::shared_ptr<frc::Counter> pwmEncoder,
                        std::shared_ptr<frc::Encoder> quadratureEncoder = nullptr,
                        bool angular = false
            ): angular_(angular), quad_(quadratureEncoder), pwm_(pwmEncoder) { pwm_->SetSemiPeriodMode(true); }

            /// @brief Returns true if this is measuring an angle
            /// @returns true if this is measuring an angle
            bool isAngular() { return angular_; };

            /// @brief returns true if this encoder contains a quadrature encoder
            /// @returns true if this XeroEncoder has a quadrature encoder
            bool hasQuadrature() {
                return quad_ != nullptr ;
            }

            /// @brief returns true if this encoder contains an absoluate encoder (analog or pwm)
            /// @returns true if this XeroEncoder has a absoluate encoder (analog of pwm)
            bool hasAbsolute() {
                return analog_ != nullptr || pwm_ != nullptr ;
            }

            /// @brief Returns the current position of this encoder.
            /// If this encoder uses both a quadrature and absolute encoder,
            /// the value measured by the quadrature encoder is returned.
            /// @returns the current position of the mechanism
            double getPosition();

            /// @brief Returns the absolute position of the encoder.
            /// If this encoder has an absolute encoder, returns the measured position.
            /// Asserts otherwise.
            /// @returns the position as read from the absolute encoder
            double getAbsolutePosition();

            /// @brief Resets the quadrature encoder to the zero position. 
            /// If an absolute encoder is declared, the position parameters 
            /// are updated to calibrate the encoders. If no quadrature 
            /// encoder is in use, this method has no effect.
            void reset();

            /// @brief calibrates the quadrature encoder based on the value of the absoluate encoder
            /// If this encoder uses both a quadrature and absolute encoder,
            /// this resets the B value of the quadrature encoder
            /// such that its current value matches the absolute encoder.
            ///
            /// If this encoder does not use both a quad & absolute encoder,
            /// this method has no effect.
            void calibrate();

            /// @brief calibrates the quadrature encoder based on an externally supplied position
            /// @param pos the current position of the measured mechanism
            void calibrate(double pos) ;

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

            void setSpecialCaseFixMe()
            {
                special_case_fix_me_ = true ;
            }
        private:
            bool angular_;

            std::shared_ptr<frc::Encoder> quad_;    // A quadrature encoder, or nullptr.
            double quadM_ = 1;
            double quadB_ = 0;

            std::shared_ptr<frc::AnalogInput> analog_;    // An analog encoder, or nullptr.
            std::shared_ptr<frc::Counter> pwm_;    // A PWM encoder, or nullptr.
            double absM_ = 1;
            double absB_ = 0;

            std::string name_ ;

            bool special_case_fix_me_ ;
        };
    }
}
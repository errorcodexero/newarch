#pragma once

#include "GoPiGo3SPI.h"
#include <memory>

namespace gopigo3
{
    /// This class is used to control the GoPiGo3 robot.  It provides low level
    /// initialization functions and low level control functions.
    class GoPiGo3Robot
    {
    public:

        //
        // Various LEDs on the robot
        //
        
        /// \brief index for the front left "eye" LED
        static const uint8_t LED_EYE_LEFT = 0x02;

        /// \brief index for the front right "eye" LED
        static const uint8_t LED_EYE_RIGHT = 0x01;

        /// \brief index for the rear left "blinker" LED
        static const uint8_t LED_BLINKER_LEFT = 0x04;

        /// \brief index for the rear right "blinker" LED
        static const uint8_t LED_BLINKER_RIGHT = 0x08;

        //
        // The two motors on the robot
        //

        /// \brief index for the left motor and encoder
        static const uint8_t MOTOR_LEFT = 0x01;

        /// \brief index for the right motor and encoder
        static const uint8_t MOTOR_RIGHT = 0x02;

        /// \brief The status of a robot motor
        struct MotorStatus
        {
            uint8_t State;          ///< The state of the motor (state flags defined below)
            int8_t Power;           ///< The power applied to the motor (-100 to 100)
            int32_t Position;       ///< The position of the motor as read from the encoder
            int16_t DPS;            ///< The speed of the motor in degress per second (0 - 1000)
        };

        /// \brief This flag indicates a motor is off due to battery low voltage
        static const uint8_t LOW_VOLTAGE_FLOAT = 0x01;

        /// \brief This flag indicates a motor cannot reach is target speed
        static const uint8_t OVERLOADED = 0x02;

        /// \brief The number of ticks per rotation for the magnetic encoders
        static const int TicksPerRotation = 6;

        /// \brief The index for the first servo
        static const uint8_t SERVO_1 = 0x01;

        /// \brief The index for the second servo
        static const uint8_t SERVO_2 = 0x02;

    public:
        /// \brief Create the robot object.
        ///
        /// At this point the robot is not initialized and init must be called
        /// to establish communications with the robot
        GoPiGo3Robot();

        /// \brief Destroy the robot object
        ///
        /// This method closes the connection to the robot board
        ~GoPiGo3Robot();

        /// \brief Initialize the robot and ready for operation
        ///
        /// This method creates the SPI port connection and queries the robot for
        /// various identification information to ensure that the hardware is compatible with
        /// this software.
        /// \param strict if true, the manufacturer and product id must match
        void init(bool strict=true);

        /// \brief Return the manufacturers string
        const std::string &getManufacturer() const {
            return m_manufacturer;
        }

        /// \brief Return the product id string
        const std::string &getProductId() const {
            return m_board;
        }

        /// \brief Return the hardware version number
        const std::uint32_t getHardwareVersion() const {
            return m_hw_version;
        }

        /// \brief Return the hardware version as a string
        const std::string getHardwareVersionString() const {
            return versionToString(m_hw_version);
        }

        /// \brief Return the firmware version number
        const std::uint32_t getFirmwareVersion() const {
            return m_fw_version;
        }

        /// \brief Return the firmware version as a string
        const std::string getFirmwareVersionString() const {
            return versionToString(m_fw_version);
        }

        /// \brief Set a specific led to a specific color
        /// \param the led index for the index to update
        /// \param the red value of the led
        /// \param the green value of the led
        /// \param the blue value of the led
        void setLed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);

        /// \brief Return the last RGB setting for the given LED
        /// \param the led index for the index to update
        /// \returns a constant vector with the RGB settings
        std::vector<uint8_t> getLastLedSetting(uint8_t led);

        /// \brief Return the actual voltage of the 5v supply
        float getVoltage5V();

        /// \brief Return the battery voltage
        float getVoltageBattery();

        /// \brief Get the endoder value for the given encoder
        /// \param which the encoder to get the value for, MOTOR_LEFT or MOTOR RIGHT
        int32_t getEncoder(uint8_t which);

        /// \brief offset the encoder for a given encoder by the offset given
        /// \param which the encoder to set the value for, MOTOR_LEFT or MOTOR RIGHT
        /// \param value the offset value to add to the current value
        void offsetEncoder(uint8_t which, int32_t value);

        /// \brief reset a given encoder to zero
        /// \param which the encoder to reset to zero
        void resetEncoder(uint8_t which);

        /// \brief return the status of a given motor
        /// \param which the motor of interest, MOTOR_LEFT or MOTOR RIGHT
        /// \param the status of the motor
        void getMotorStatus(uint8_t which, MotorStatus &st);

        /// \brief set the limits of a given motor
        /// \param which the motor of interest, MOTOR_LEFT or MOTOR RIGHT
        /// \param the power limit (0 - 100) with 0 meaning unlimited
        /// \param the speed limit (0 - 100) with 0 meaning unlimited
        void setMotorLimits(uint8_t which, uint8_t power, uint16_t dps);

        /// \brief set the power of a given motor
        /// \param which the motor of interest, MOTOR_LEFT or MOTOR RIGHT
        /// \param the power value (-100 to 100)
        void setMotorPower(uint8_t which, int8_t power);

        /// \brief set the servo position
        /// \param which the servo of interest, SERVO_1 or SERVO_2
        /// \param angle the angle of the servo in  microseconds (0 - 16666)
        void setServo(uint8_t which, uint16_t angle);

        /// \brief turn off all robot functions, motor LEDs, etc.
        void reset(bool verbose);

    private:
        //
        // Convert a version number to a string, used for the HW and FW versions
        static std::string versionToString(uint32_t ver)  {
            std::string ret;

            ret = std::to_string(ver / 1000000);
            ret += ".";
            ret += std::to_string((ver / 1000) % 1000);
            ret += ".";
            ret += std::to_string(ver % 1000);

            return ret;
        }

    private:
        //
        // Name of the SPI port to communicate with the robot board
        //
        const char *m_spi_port_name = "/dev/spidev0.1";

        //
        // Maximum SPI message size in bytes
        //
        const uint8_t m_spi_buf_size = 24;

        //
        // The robot board address
        //
        const uint8_t m_robot_board_address = 8;

        //
        // Expected name of the manufacturer
        //
        const char *m_expected_manufacturer = "Dexter Industries";

        //
        // Expected name of the board
        //
        const char *m_expected_board = "GoPiGo3";

        //
        // Messages the robot board understands via SPI
        //
        enum GPGSPI_MESSAGE_TYPE {
            GPGSPI_MESSAGE_NONE,

            GPGSPI_MESSAGE_GET_MANUFACTURER,
            GPGSPI_MESSAGE_GET_NAME,
            GPGSPI_MESSAGE_GET_HARDWARE_VERSION,
            GPGSPI_MESSAGE_GET_FIRMWARE_VERSION,
            GPGSPI_MESSAGE_GET_ID,

            GPGSPI_MESSAGE_SET_LED,

            GPGSPI_MESSAGE_GET_VOLTAGE_5V,
            GPGSPI_MESSAGE_GET_VOLTAGE_VCC,

            GPGSPI_MESSAGE_SET_SERVO,

            GPGSPI_MESSAGE_SET_MOTOR_PWM,

            GPGSPI_MESSAGE_SET_MOTOR_POSITION,
            GPGSPI_MESSAGE_SET_MOTOR_POSITION_KP,
            GPGSPI_MESSAGE_SET_MOTOR_POSITION_KD,

            GPGSPI_MESSAGE_SET_MOTOR_DPS,

            GPGSPI_MESSAGE_SET_MOTOR_LIMITS,

            GPGSPI_MESSAGE_OFFSET_MOTOR_ENCODER,

            GPGSPI_MESSAGE_GET_MOTOR_ENCODER_LEFT,
            GPGSPI_MESSAGE_GET_MOTOR_ENCODER_RIGHT,

            GPGSPI_MESSAGE_GET_MOTOR_STATUS_LEFT,
            GPGSPI_MESSAGE_GET_MOTOR_STATUS_RIGHT,

            GPGSPI_MESSAGE_SET_GROVE_TYPE,
            GPGSPI_MESSAGE_SET_GROVE_MODE,
            GPGSPI_MESSAGE_SET_GROVE_STATE,
            GPGSPI_MESSAGE_SET_GROVE_PWM_DUTY,
            GPGSPI_MESSAGE_SET_GROVE_PWM_FREQUENCY,

            GPGSPI_MESSAGE_GET_GROVE_VALUE_1,
            GPGSPI_MESSAGE_GET_GROVE_VALUE_2,
            GPGSPI_MESSAGE_GET_GROVE_STATE_1_1,
            GPGSPI_MESSAGE_GET_GROVE_STATE_1_2,
            GPGSPI_MESSAGE_GET_GROVE_STATE_2_1,
            GPGSPI_MESSAGE_GET_GROVE_STATE_2_2,
            GPGSPI_MESSAGE_GET_GROVE_VOLTAGE_1_1,
            GPGSPI_MESSAGE_GET_GROVE_VOLTAGE_1_2,
            GPGSPI_MESSAGE_GET_GROVE_VOLTAGE_2_1,
            GPGSPI_MESSAGE_GET_GROVE_VOLTAGE_2_2,
            GPGSPI_MESSAGE_GET_GROVE_ANALOG_1_1,
            GPGSPI_MESSAGE_GET_GROVE_ANALOG_1_2,
            GPGSPI_MESSAGE_GET_GROVE_ANALOG_2_1,
            GPGSPI_MESSAGE_GET_GROVE_ANALOG_2_2,

            GPGSPI_MESSAGE_START_GROVE_I2C_1,
            GPGSPI_MESSAGE_START_GROVE_I2C_2
        };


    private:
        //
        // The SPI hardware connection
        //
        std::shared_ptr<GoPiGo3SPI> m_spi_p;

        //
        // The ID strings for the robot
        //
        std::string m_manufacturer;
        std::string m_board;

        //
        // The versions for the robot
        //
        uint32_t m_hw_version;
        uint32_t m_fw_version;

        std::vector<uint8_t> led_left_eye_;
        std::vector<uint8_t> led_right_eye_;
        std::vector<uint8_t> led_left_blink_;
        std::vector<uint8_t> led_right_blink_;
    };
}

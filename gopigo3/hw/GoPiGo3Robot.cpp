#include "GoPiGo3Robot.h"
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>

namespace gopigo3
{
    GoPiGo3Robot::GoPiGo3Robot()
    {
        led_left_blink_.resize(3);
        std::fill(led_left_blink_.begin(), led_left_blink_.end(), 0);

        led_right_blink_.resize(3);
        std::fill(led_right_blink_.begin(), led_right_blink_.end(), 0);

        led_left_eye_.resize(3);
        std::fill(led_left_eye_.begin(), led_left_eye_.end(), 0);

        led_right_eye_.resize(3);
        std::fill(led_right_eye_.begin(), led_right_eye_.end(), 0);
    }

    GoPiGo3Robot::~GoPiGo3Robot()
    {
        reset(false);
    }

    void GoPiGo3Robot::reset(bool verbose)
    {
        uint8_t target;

        if (m_spi_p != nullptr)
        {
            std::cout << "        Turning off LEDs ... " << std::flush;
            target = LED_BLINKER_LEFT | LED_BLINKER_RIGHT | LED_EYE_LEFT | LED_EYE_RIGHT;
            setLed(target, 0, 0, 0);
            std::cout << "done." << std::endl;

            std::cout << "        Turning off drivebase motors ... " << std::flush;
            target = MOTOR_LEFT | MOTOR_RIGHT;
            setMotorPower(target, 0);
            std::cout << "done." << std::endl;

            std::cout << "        Turning off servos ... " << std::flush;
            target = SERVO_1 | SERVO_2;
            setServo(target, 0);
            std::cout << "done." << std::endl;
        }
    }

    void GoPiGo3Robot::init(bool strict)
    {
        std::string str;
        std::chrono::milliseconds delay(500);

        wiringPiSetup();

        //
        // Write the Raspiberry PI GPIO which enables the robot
        //
        pinMode(4, OUTPUT);
        digitalWrite(4, HIGH);
        std::this_thread::sleep_for(delay);

        //
        // Create the SPI port to talk to the daughter processor
        //
        m_spi_p = std::make_shared<GoPiGo3SPI>(m_spi_port_name, m_spi_buf_size);
        m_spi_p->setAddress(m_robot_board_address);

        //
        // Check the product information
        //
        m_manufacturer =  m_spi_p->readString(GPGSPI_MESSAGE_GET_MANUFACTURER);
        m_board = m_spi_p->readString(GPGSPI_MESSAGE_GET_NAME);
        m_hw_version = m_spi_p->read32(GPGSPI_MESSAGE_GET_HARDWARE_VERSION);
        m_fw_version = m_spi_p->read32(GPGSPI_MESSAGE_GET_FIRMWARE_VERSION);

        if (strict && m_manufacturer != m_expected_manufacturer)
        {
            std::string msg;
            msg = "expected manufacturer '";
            msg += m_expected_manufacturer;
            msg += "' but got '";
            msg += m_manufacturer;
            msg += "'";
            std::runtime_error err(msg.c_str());
            throw err;
        }

        if (strict && m_board != m_expected_board)
        {
            std::string msg;
            msg = "expected product id '";
            msg += m_expected_board;
            msg += "' but got '";
            msg += m_board;
            msg += "'";
            std::runtime_error err(msg.c_str());
            throw err;
        }
    }

    void GoPiGo3Robot::setLed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
        std::vector<uint8_t> buf(4);
        buf[0] = led;
        buf[1] = red; 
        buf[2] = green;
        buf[3] = blue;
        m_spi_p->sendMessage(GPGSPI_MESSAGE_SET_LED, buf);

        switch(led)
        {
        case LED_BLINKER_LEFT:
            led_left_blink_[0] = red;
            led_left_blink_[1] = green;
            led_left_blink_[2] = blue;
            break;
        case LED_BLINKER_RIGHT:
            led_right_blink_[0] = red;
            led_right_blink_[1] = green;
            led_right_blink_[2] = blue;
            break;
        case LED_EYE_LEFT:
            led_left_eye_[0] = red;
            led_left_eye_[1] = green;
            led_left_eye_[2] = blue;
            break;
        case LED_EYE_RIGHT:
            led_right_eye_[0] = red;
            led_right_eye_[1] = green;
            led_right_eye_[2] = blue;
            break;
        }
    }

    std::vector<uint8_t> GoPiGo3Robot::getLastLedSetting(uint8_t led)
    {
        std::vector<uint8_t> result;

        switch (led)
        {
        case LED_BLINKER_LEFT:
            return led_left_blink_;
        case LED_BLINKER_RIGHT:
            return led_right_blink_;
        case LED_EYE_LEFT:
            return led_left_eye_;
        case LED_EYE_RIGHT:
            return led_right_eye_;
        }

        return result;
    }

    float GoPiGo3Robot::getVoltage5V()
    {
        uint16_t value = m_spi_p->read16(GPGSPI_MESSAGE_GET_VOLTAGE_5V);
        return static_cast<float>(value) / 1000.0f;
    }

    float GoPiGo3Robot::getVoltageBattery()
    {
        uint16_t value = m_spi_p->read16(GPGSPI_MESSAGE_GET_VOLTAGE_VCC);
        return static_cast<float>(value) / 1000.0f;
    }

    void GoPiGo3Robot::offsetEncoder(uint8_t which, int32_t value)
    {
        if (which != MOTOR_LEFT && which != MOTOR_RIGHT)
        {
            std::runtime_error err("offsetEncoder - invalid motor value supplied");
            throw err;
        }

        std::vector<uint8_t> data(5);
        data[0] = which;
        data[1] = static_cast<uint8_t>(((value >> 24) & 0xFF));
        data[2] = static_cast<uint8_t>(((value >> 16) & 0xFF));
        data[3] = static_cast<uint8_t>(((value >>  8) & 0xFF));
        data[4] = static_cast<uint8_t>(((value >>  0) & 0xFF));
        m_spi_p->sendMessage(GPGSPI_MESSAGE_OFFSET_MOTOR_ENCODER, data);
    }

    int32_t GoPiGo3Robot::getEncoder(uint8_t which)
    {
        uint8_t msg;

        if (which == MOTOR_LEFT)
        {
            msg = GPGSPI_MESSAGE_GET_MOTOR_ENCODER_LEFT;
        }
        else if (which == MOTOR_RIGHT)
        {
            msg = GPGSPI_MESSAGE_GET_MOTOR_ENCODER_RIGHT;
        }
        else
        {
            std::runtime_error err("getEncoderValue - invalid motor value supplied");
            throw err;
        }

        return static_cast<int32_t>(m_spi_p->read32(msg));
    }

    void GoPiGo3Robot::resetEncoder(uint8_t which)
    {
        int32_t value = getEncoder(which);
        offsetEncoder(which, value);
    }

    void GoPiGo3Robot::getMotorStatus(uint8_t which, MotorStatus &st)
    {
        uint8_t msg;

        if (which == MOTOR_LEFT)
        {
            msg = GPGSPI_MESSAGE_GET_MOTOR_STATUS_LEFT;
        }
        else if (which == MOTOR_RIGHT)
        {
            msg = GPGSPI_MESSAGE_GET_MOTOR_STATUS_RIGHT;
        }
        else
        {
            std::runtime_error err("getMotorStatus - invalid motor value supplied");
            throw err;
        }

        std::vector<uint8_t> obuf;
        std::vector<uint8_t> ibuf(12);
        m_spi_p->sendMessage(msg, obuf, ibuf);

        st.State = ibuf[4];
        st.Power = ibuf[5];
        st.Position = (ibuf[6] << 24) | (ibuf[7] << 16) | (ibuf[8] << 8) | ibuf[9];
        st.DPS = static_cast<int16_t>((ibuf[10] << 8) | ibuf[11]);
    }

    void GoPiGo3Robot::setMotorPower(uint8_t which, int8_t power)
    {
        std::vector<uint8_t> packet(2);
        packet[0] = which;
        packet[1] = power;

        m_spi_p->sendMessage(GPGSPI_MESSAGE_SET_MOTOR_PWM, packet);
    }

    void GoPiGo3Robot::setMotorLimits(uint8_t which, uint8_t power, uint16_t dps)
    {
        std::vector<uint8_t> packet(4);
        packet[0] = which;
        packet[1] = power;
        packet[2] = static_cast<uint8_t>((dps >> 8) & 0xff);
        packet[3] = static_cast<uint8_t>(dps & 0xff);

        m_spi_p->sendMessage(GPGSPI_MESSAGE_SET_MOTOR_LIMITS, packet);
    }

    void GoPiGo3Robot::setServo(uint8_t which, uint16_t angle)
    {
        std::vector<uint8_t> packet(3);
        packet[0] = which;
        packet[1] = static_cast<uint8_t>((angle >> 8) & 0xff);
        packet[2] = static_cast<uint8_t>(angle & 0xff);

        m_spi_p->sendMessage(GPGSPI_MESSAGE_SET_SERVO, packet);
    }
}

#include <frc/Encoder.h>
#include <GoPiGo3Robot.h>
#include <frc/RobotBase.h>
#include <stdexcept>

namespace frc
{
    Encoder::Encoder(uint32_t aChannel, uint32_t bChannel, bool reverseDirection, EncodingType encodingType)
    {
        m_type = encodingType;
        inverted_ = false ;

        if (aChannel == 0 && bChannel == 1)
            m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_LEFT;
        else if (aChannel == 2 && bChannel == 3)
            m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_RIGHT;
        else
        {
            std::runtime_error err("illegal encoder channel combination, only 0 & 1 (left) and 2 & 3 (right) supported");
            throw err;
        }
    }

    Encoder::~Encoder()
    {
    }

    int32_t Encoder::Get() const
    {
        gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
        int32_t value = hw.getEncoder(m_hw_channel);

        if (inverted_)
            value = -value ;

        return value ;
    }

    int32_t Encoder::GetEncodingScale() const
    {
        return 1;
    }

    void Encoder::Reset()
    {
        gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
        return hw.resetEncoder(m_hw_channel);
    }
}
